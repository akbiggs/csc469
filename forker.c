#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "tsc.h"
#include "common.h"
#include "forker.h"


int main(int argc, char** argv) {
	
	if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_samples>\n", argv[0]);
        return -1;
    }

    int num_samples = atoi(argv[1]);
    u_int64_t samples[num_samples * 2];
    int threshold = find_threshold(5);
    start_counter();
    
    int first_pipe[2];
    pipe(first_pipe);

    // fork two children processes to run
    switch (fork()) {
        case -1:
            fprintf(stderr, "Could not fork first child.\n");
            return 1;
        case 0:
            // in the first child
            close(first_pipe[0]); // close reader
            run_child("first", num_samples, first_pipe, threshold, samples);
            close(first_pipe[1]); // close writer
            break;

        default:
			// in the parent
			close(first_pipe[1]); // close writer
			
			int second_pipe[2];
			pipe(second_pipe);
            
            switch (fork()) {
                case -1:
                    fprintf(stderr, "Could not fork second child.\n");
                case 0:
                    // in the second child
                    close(first_pipe[0]); // close reader
					run_child("secnd", num_samples, second_pipe, threshold, samples);
					close(first_pipe[1]); // close writer
                    break;
                default:
                    // still in the parent
                    close(second_pipe[1]);  // close writer
                    
                    run_parent(num_samples, first_pipe, second_pipe);
                    close(first_pipe[0]);  // close reader
                    close(second_pipe[0]);  // close reader
                    break;
            }

            break;
    }

    return 0;
}

void run_parent(int num_samples, int* first_pipe, int* second_pipe) {
	
	u_int64_t first_read_buffer[num_samples * 2];
	u_int64_t first_start;
	read(first_pipe[0], &first_start, sizeof(u_int64_t));
	read(first_pipe[0], first_read_buffer, sizeof(u_int64_t) * num_samples * 2);
	
	u_int64_t second_read_buffer[num_samples * 2];
	u_int64_t second_start;
	read(second_pipe[0], &second_start, sizeof(u_int64_t));
	read(second_pipe[0], second_read_buffer, sizeof(u_int64_t) * num_samples * 2);
	
	// Combine the two sorted lists into one sorted list.
	u_int64_t combined_list[num_samples * 4];
	int combined_list_owner[num_samples * 4];
	int first_i = 0, second_i = 0, comb_i = 0;
	for (comb_i = 0; comb_i < num_samples * 4; comb_i += 1) {
		if (second_i >= num_samples * 2 || (first_i < num_samples * 2 && first_read_buffer[first_i] < second_read_buffer[second_i])) {
			combined_list[comb_i] = first_read_buffer[first_i];
			combined_list_owner[comb_i] = 0 + first_i % 2;
			first_i += 1;
		} else {
			combined_list[comb_i] = second_read_buffer[second_i];
			combined_list_owner[comb_i] = 2 + second_i % 2;
			second_i += 1;
		}
	}
	
	int i;
	printf("1 is active at %" PRIu64 "\n", first_start);
	printf("2 is active at %" PRIu64 "\n", second_start);
	int sum_cs = 0;
	int num_cs = 0;
	for (i = 0; i < num_samples * 4; i += 1) {
		char* description;
		switch (combined_list_owner[i]) {
			case 0: description = "1 is inactive"; break;
			case 1: description = "1 is active  "; break;
			case 2: description = "2 is inactive"; break;
			case 3: description = "2 is active  "; break;
		}
		printf("%s at %" PRIu64 "\n", description, combined_list[i]);
		
		if (i > 0) {
			if ((combined_list_owner[i-1] == 0 && combined_list_owner[i] == 3) || (combined_list_owner[i-1] == 2 && combined_list_owner[i] == 1)) {
				sum_cs = combined_list[i] - combined_list[i-1];
				num_cs++;
			}
		}
	}
	
	printf("ESTIMATED CONTEXT SWITCH TIME IS %f MS\n", cycles_to_ms(sum_cs / num_cs));
	plot_samples("forker_plot.sh", first_read_buffer, second_read_buffer, num_samples, cycles_to_ms(first_start), cycles_to_ms(second_start));
}

void run_child(char* name, int num_samples, int* pipe, int threshold, u_int64_t* samples) {	
	u_int64_t start = inactive_periods(num_samples, threshold, samples);
	//u_int64_t start = detect_context_switch_periods(num_samples, samples);
	write(pipe[1], &start, sizeof(u_int64_t));
	write(pipe[1], samples, sizeof(u_int64_t) * num_samples * 2);
}
    
u_int64_t detect_context_switch_periods(int num, u_int64_t *samples) {
	u_int64_t start_active = get_counter();

    u_int64_t prev_counter = start_active;
    u_int64_t cur_counter = start_active;
    
    int num_sampled = 0;
    while (num_sampled < num) {
		
		detect_context_switch(&prev_counter, &cur_counter);

        samples[num_sampled*2] = prev_counter;
        samples[num_sampled*2 + 1] = cur_counter;

        num_sampled++;

        // make counters equal to avoid having the time taken spent printing
        // out information, adding samples and incrementing being taken into
        // account
        prev_counter = get_counter();
        cur_counter = prev_counter;
    }

    return start_active;
}

int detect_context_switch(u_int64_t* prev_counter, u_int64_t* cur_counter) {
	struct rusage prev_r, cur_r;
	getrusage(RUSAGE_SELF, &prev_r);
	getrusage(RUSAGE_SELF, &cur_r);
	
	int count = 0;
	while (cur_r.ru_nivcsw <= prev_r.ru_nivcsw) {
		*prev_counter = *cur_counter;
		*cur_counter = get_counter();
		
		prev_r = cur_r;
		getrusage(RUSAGE_SELF, &cur_r);
		count++;
	}
    return count;
}

/**
 * Produces a bash file that generates a graph for gnuplot.
 */ 
int plot_samples(char* filename, u_int64_t* first_samples, u_int64_t* second_samples, int num_samples, float start_first, float start_second) {
    FILE* plot_file = fopen(filename, "w");
    if (!plot_file) {
        return -1;
    }

    fputs("#!/bin/sh\n", plot_file);
    fputs("gnuplot << ---EOF---\n", plot_file);
    fputs("set title \"Active and Inactive Periods\"\n", plot_file);
    fputs("set xlabel \"Time (ms)\"\n", plot_file);
    fputs("set nokey\n", plot_file);
    fputs("set noytics\n", plot_file);
    fputs("set label \"Inactive\" at graph 0.95, graph 0.95 right tc lt 1 font \"Arial,7\"\n", plot_file);
    fputs("set label \"Child 1 Active\" at graph 0.95, graph 0.9 right tc lt 3 font \"Arial,7\"\n", plot_file);
    fputs("set label \"Child 2 Active\" at graph 0.95, graph 0.85 right tc lt 2 font \"Arial,7\"\n", plot_file);
    fputs("set term postscript eps 10\n", plot_file);
    fputs("set size 0.45,0.35\n", plot_file);
    fputs("set output \"forker.eps\"\n", plot_file);

    float millis_elapsed = start_first;
    float cumulative_millis = start_first;
    
    if (num_samples != 0) {
        
        // Plot the results from the first pipe.
        int i;
        for (i = 0; i < num_samples * 2; i++) {

            u_int64_t next_time = first_samples[i];
			char* color;
            if (i % 2 == 0) {
                color = "blue"; // active
            } else {
				color = "red"; // inactive
            }
            
            cumulative_millis = cycles_to_ms(next_time);

			fprintf(plot_file, "set object %d rect from %f, 2 to %f, 3 fc rgb \"%s\" fs solid noborder\n", i + 1, millis_elapsed, cumulative_millis, color);
			millis_elapsed = cumulative_millis;
        }
        
        // Plot the results from the second pipe.
        millis_elapsed = start_second;
		cumulative_millis = start_second;
        for (i = 0; i < num_samples * 2; i++) {

            u_int64_t next_time = second_samples[i];
			char* color;
            if (i % 2 == 0) {
                color = "green"; // active
            } else {
				color = "red"; // inactive
            }
            
            cumulative_millis = cycles_to_ms(next_time);

			fprintf(plot_file, "set object %d rect from %f, 1 to %f, 2 fc rgb \"%s\" fs solid noborder\n", num_samples * 2 + i + 1, millis_elapsed, cumulative_millis, color);
			millis_elapsed = cumulative_millis;
        }
    }

    fprintf(plot_file, "plot [%f:%f] [0:4] 0\n", start_first, cumulative_millis);
    fputs("---EOF---\n", plot_file);

    fclose(plot_file);

    return 0;
}
