#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

#include "tracker.h"
#include "tsc.h"

// TODO: figure out gnuplot
// TODO: write run_test_experiment_A bash file
// TODO: write the threshold finder code
// TODO: do clock to ms
// TOOD: Understand how proc/interupts matters

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_samples> <threshold>\n", argv[0]);
        return -1;
    }

    int num_samples = atoi(argv[1]);
    int threshold = atoi(argv[2]);
    u_int64_t samples[num_samples*2];
    
    test_cache_miss_time();
    find_threshold(num_samples);

    start_counter();
    u_int64_t start = inactive_periods(num_samples, threshold, samples);

    int i;
    for (i = 0; i < num_samples; i++) {
       u_int64_t active_dur = samples[i*2] - start;
       u_int64_t inactive_dur = samples[i*2+1] - samples[i*2];

       printf("Active %d: start at %" PRIu64 ", duration %" PRIu64 " cycles (%0.6f ms)\n",
               i, start, active_dur, cycles_to_ms(active_dur));
       printf("Inactive %d: start at %" PRIu64 ", duration %" PRIu64 " cycles (%0.6f ms)\n",
               i, start + active_dur, inactive_dur, cycles_to_ms(inactive_dur));

       start += active_dur + inactive_dur;
    }

    if (plot_samples("runplot.sh", samples, num_samples, cycles_to_ms(start))) {
        fprintf(stderr, "Failed to write samples to a gnuplot file.\n");
    }

    return 0;
}

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples) {
    u_int64_t start_active = get_counter();

    u_int64_t prev_counter = start_active;
    u_int64_t cur_counter = start_active;
    
    int num_sampled = 0;
    while (num_sampled < num) {
		
		wait_until_inactive_period(&prev_counter, &cur_counter);

        //printf("adding sample %d\n", num_sampled);

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

int wait_until_inactive_period(u_int64_t* prev_counter, u_int64_t* cur_counter) {
	int cycle_count = 0;
	while (cur_counter - prev_counter <= threshold) {
		*prev_counter = *cur_counter;
		*cur_counter = get_counter();
		count++;
	}
    return cycle_count;
}

int find_threshold(int num) { 
	int iterations_per_threshold = 5000;
	int min_threshold = 40;
	int max_threshold = 500;
	int threshold_sample_interval = 2;
	
	printf("== FINDING THRESHOLD ==\nFinding the ideal threshold from %d to %d.\n", min_threshold, max_threshold);
	
	// We iterate through all the thresholds to find the ideal one.
	int threshold;
	int last_inactive_count = 0;
	for (threshold = min_threshold; threshold < max_threshold; threshold += threshold_sample_interval) {
		
		// Initalize the counter variables.
		u_int64_t cur_counter = get_counter();
		u_int64_t prev_counter = cur_counter;
		
		int i = 0;
		int inactive_count = 0;
		int sum_diff = 0;
		
		// Run the same that will be ran when finding inactive periods.
		int cycles = 0;
		int periods = 0;
		while (periods < num) {
			cycles += wait_until_inactive_period(&cur_counter, &cur_counter);
			periods++;
		}
		
		for (i = 0; i < iterations_per_threshold; i++) {
			cur_counter = get_counter();
			sum_diff += cur_counter - prev_counter;
			if (cur_counter - prev_counter > threshold) {
				inactive_count++;
			}
			int j;
			for (j = 0; j < 100; j++) { }
			prev_counter = cur_counter;
		}
		int average_diff = sum_diff / iterations_per_threshold;
		if (inactive_count != last_inactive_count) {
			printf("Threshold %d: %d inactive periods (average_diff = %d / %d = %d)\n", threshold, inactive_count, sum_diff, iterations_per_threshold, average_diff);
		}
		last_inactive_count = inactive_count;
	}
	return 1;
}

float cycles_to_ms(u_int64_t cycles) {
    // TODO: Experiment to figure out how these actually relate.
    return (float)(cycles / 100000.0f);
}

/**
 * Tests cache miss time by comparing array gets and sets when accessing the
 * same element repeatidly versus accessing random elements.
 */
void test_cache_miss_time() {
	srand(time(NULL));
	int i, j;
	int array_size = 100;
	int** array = (int **)malloc(sizeof(int *)*array_size);
	for(i = 0; i < array_size; i++) {
	  array[i] = (int *)malloc(sizeof(int)*array_size);
	}
	int averaging_iterations = 10100100;
	printf("== TESTING CACHE MISS TIME ==\nArray of size %d * %d averaging over %d iterations.\n", array_size, array_size, averaging_iterations);
	
	int zero = 0;
	zero += 1;
	zero -= 1;
	u_int64_t prev_counter, cur_counter;
	
	// Warm up the cache...
	int sum = 0;
	for (i = 0; i < array_size; i++) {
		for (j = 0; j < array_size; j++) {
			sum += array[i][j];
		}
	}
	
	// Test the time to get and set the same element over and over.
	prev_counter = get_counter();
	increment_array(array, averaging_iterations, 1);
	cur_counter = get_counter();
	int average_repeat = (cur_counter - prev_counter) / averaging_iterations;
	printf("Repeated Access -- %d cycles on average\n", average_repeat);

	// Test the time to get and set random elements over and over.
	prev_counter = get_counter();
	increment_array(array, averaging_iterations, array_size);
	cur_counter = get_counter();
	int average_random = (cur_counter - prev_counter) / averaging_iterations;
	printf("Random Access -- %d cycles on average\n", average_random);

	// Add up the values so the compiler knows we need them.
	sum = 0;
	for (i = 0; i < array_size; i++) {
		for (j = 0; j < array_size; j++) {
			sum += array[i][j];
		}
	}
	for(i = 0; i < array_size; i++) {
	  free(array[i]);
	}
	free(array);
	
	printf("Sum of array -- %d\n", sum);
	printf("Cache miss time is roughly -- %d\n", average_random - average_repeat);
}

void increment_array(int** array, int increments, int max_array_index_select) {
	int i;
	for (i = 0; i < increments; i++) {
		int posx = rand() % max_array_index_select;
		int posy = rand() % max_array_index_select;
		array[posx][posy] += 1;
	}
}

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

	    fprintf(plot_file, "set object %d rect from %f, 1 to %f, 2 fc rgb \"%s\" fs solid\n",
		    i + 1, millis_elapsed, cumulative_millis, color);
	    
	    millis_elapsed = cumulative_millis;
        }
    }

    fprintf(plot_file, "plot [0:%f] [0:3] 0\n", cumulative_millis);
    fputs("---EOF---\n", plot_file);

    fclose(plot_file);

    return 0;
}
