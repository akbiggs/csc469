#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

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
	for (comb_i = 0; comb_i < num_samples * 4; comb_i += 2) {
		if (second_i >= num_samples * 2 || (first_i < num_samples * 2 && first_read_buffer[first_i] < second_read_buffer[second_i])) {
			combined_list[comb_i] = first_read_buffer[first_i];
			combined_list[comb_i+1] = first_read_buffer[first_i+1];
			first_i += 2;
			combined_list_owner[comb_i] = 0;
			combined_list_owner[comb_i+1] = 0;
		} else {
			combined_list[comb_i] = second_read_buffer[second_i];
			combined_list[comb_i+1] = second_read_buffer[second_i+1];
			second_i += 2;
			combined_list_owner[comb_i] = 1;
			combined_list_owner[comb_i+1] = 1;
		}
	}
	
	int i;
	printf("owner: 0 - start active %" PRIu64 "\n", first_start);
	printf("owner: 1 - start active %" PRIu64 "\n", second_start);
	for (i = 0; i < num_samples * 4; i += 2) {
		printf("owner: %d - start inactive %" PRIu64 "\n", combined_list_owner[i], combined_list[i]);
		printf("owner: %d - end inactive   %" PRIu64 "\n", combined_list_owner[i+1], combined_list[i+1]);
	}
}

void run_child(char* name, int num_samples, int* pipe, int threshold, u_int64_t* samples) {	
	u_int64_t start = inactive_periods(num_samples, threshold, samples);
	
	int i;
	printf("%s first start %" PRIu64 " !!!\n", name, start);
	for (i = 0; i < num_samples * 2; i++) {
		printf("%s sees %" PRIu64 " !!!\n", name, samples[i]);
	}
	write(pipe[1], &start, sizeof(u_int64_t));
	write(pipe[1], samples, sizeof(u_int64_t) * num_samples * 2);
}

/**
 * Produces a bash file that generates a graph for gnuplot.
 */ 
int plot_samples(char* filename, u_int64_t* samples, int num_samples, float start_ms) {
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
    fputs("set term postscript eps 10\n", plot_file);
    fputs("set size 0.45,0.35\n", plot_file);
    fputs("set output \"activeperiods.eps\"\n", plot_file);

    float millis_elapsed = 0;
    float cumulative_millis = 0;
    
    if (num_samples != 0) {
        
        int i;
        for (i = 0; i < num_samples*2; i++) {

            u_int64_t next_time = samples[i];
			char* color;
            if (i % 2 == 0) {
                color = "blue"; // active
            } else {
				color = "red"; // inactive
            }
            
            cumulative_millis = cycles_to_ms(next_time);

			fprintf(plot_file, "set object %d rect from %f, 1 to %f, 2 fc rgb \"%s\" fs solid noborder\n", i + 1, millis_elapsed, cumulative_millis, color);
			millis_elapsed = cumulative_millis;
        }
    }

    fprintf(plot_file, "plot [%f:%f] [0:3] 0\n", start_ms, cumulative_millis);
    fputs("---EOF---\n", plot_file);

    fclose(plot_file);

    return 0;
}
