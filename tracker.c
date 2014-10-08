#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>

#include "tracker.h"
#include "common.h"
#include "tsc.h"

// TODO: fix int when it should be u_int64_t
// TOOD: Understand how proc/interupts matters

/**
 * Runs the experiment and outputs gnuplot input to be graphed.
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_samples>\n", argv[0]);
        return -1;
    }

    int num_samples = atoi(argv[1]);
    u_int64_t samples[num_samples*2];
    
    test_cache_miss_time();
    int threshold = find_threshold(5);

    start_counter();
    u_int64_t start = inactive_periods(num_samples, threshold, samples);

	printf("=== TRACKING ACTIVITY ===\n");
    u_int64_t elapsed = start;
    int i;
    for (i = 0; i < num_samples; i++) {
       u_int64_t active_dur = samples[i*2] - elapsed;
       u_int64_t inactive_dur = samples[i*2+1] - samples[i*2];

       printf("Active %d: start at %" PRIu64 ", duration %" PRIu64 " cycles (%0.6f ms)\n",
               i, elapsed, active_dur, cycles_to_ms(active_dur));
       printf("Inactive %d: start at %" PRIu64 ", duration %" PRIu64 " cycles (%0.6f ms)\n",
               i, elapsed + active_dur, inactive_dur, cycles_to_ms(inactive_dur));

       elapsed += active_dur + inactive_dur;
    }

    if (plot_samples("tracker_plot.sh", samples, num_samples, cycles_to_ms(start))) {
        fprintf(stderr, "Failed to write samples to a gnuplot file.\n");
    }

    return 0;
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
	printf("=== TESTING CACHE MISS TIME ===\nArray of size %d * %d averaging over %d iterations.\n", array_size, array_size, averaging_iterations);
	
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

/**
 * Randomly increments a element in the array.
 * Used to help test cache performance.
 */
void increment_array(int** array, int increments, int max_array_index_select) {
	int i;
	for (i = 0; i < increments; i++) {
		int posx = rand() % max_array_index_select;
		int posy = rand() % max_array_index_select;
		array[posx][posy] += 1;
	}
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
    fputs("set label \"Active Periods\" at screen 0.5,0.5\n", plot_file);
    fputs("set label \"Inactive\" at 0.5,2.8 tc lt 1\n", plot_file);
    fputs("set label \"Active\" at 0.5,2.5 tc lt 3\n", plot_file);
    fputs("set noytics\n", plot_file);
    fputs("set term postscript eps 10\n", plot_file);
    fputs("set size 0.45,0.35\n", plot_file);
    fputs("set output \"tracker.eps\"\n", plot_file);

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

			fprintf(plot_file, "set object %d rect from %f, 1 to %f, 2 fc rgb \"%s\" fs solid\n", i + 1, millis_elapsed, cumulative_millis, color);
			millis_elapsed = cumulative_millis;
        }
    }

    fprintf(plot_file, "plot [%f:%f] [0:3] 0\n", start_ms, cumulative_millis);
    fputs("---EOF---\n", plot_file);

    fclose(plot_file);

    return 0;
}
