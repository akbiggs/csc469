#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include "tsc.h"
#include "common.h"

/**
 * Fills the samples array with values to indicate which cycles were
 * spent on inactive periods.
 */
u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples) {
    u_int64_t start_active = get_counter();

    u_int64_t prev_counter = start_active;
    u_int64_t cur_counter = start_active;
    
    int num_sampled = 0;
    while (num_sampled < num) {
		
		wait_until_inactive_period(threshold, &prev_counter, &cur_counter);

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


/**
 * This continues to wait until a large cycle count is found, indicating
 * an inactive period occured.
 */ 
int wait_until_inactive_period(int threshold, u_int64_t* prev_counter, u_int64_t* cur_counter) {
	int count = 0;
	while (*cur_counter - *prev_counter <= threshold) {
		*prev_counter = *cur_counter;
		*cur_counter = get_counter();
		count++;
	}
    return count;
}

/**
 * Finds a good threshold by iteratively testing a range of thresholds
 * in order to graph the average number of inactive periods found per
 * iteration. It then makes a selection from a found threshold range
 * that is long and varies little.
 */
int find_threshold(int num) {
	return 4000;
	// Set the parameters for the threshold sampling.
	int min_threshold = 50;
	int max_threshold = 3000;
	int threshold_sample_interval = 30;
	
	printf("=== FINDING THRESHOLD ===\nFinding the ideal threshold from %d to %d.\n", min_threshold, max_threshold);
	
	int samples = (max_threshold - min_threshold) / threshold_sample_interval;
	double threshold_percents[samples];
	
	// We iterate through all the thresholds to graph the time it takes for each threshold.
	int threshold, index = 0;
	for (threshold = min_threshold; threshold < max_threshold; threshold += threshold_sample_interval) {
		
		// Initalize the counter variables.
		u_int64_t cur_counter = get_counter();
		u_int64_t prev_counter = cur_counter;
		
		// Run the same code that will be ran when finding inactive periods.
		int iterations = 0;
		int periods = 0;
		while (periods < num) {
			iterations += wait_until_inactive_period(threshold, &prev_counter, &cur_counter);
			periods++;
			prev_counter = get_counter();
			cur_counter = prev_counter;
		}

		double percent = (num * 100) / (double)iterations;
		//printf("Threshold %d: %d inactive periods in %d iterations. %f%% inactive periods per iteration\n", threshold, num, iterations, percent);
		
		threshold_percents[index] = percent;
		index++;
	}
	
	
	// Set the parameters for the threshold search...
	double desired_deviation = 0.005;
	int desired_total = 25;
	
	// Examine the performance for the found thresholds and selects a threshold.
	int found_threshold = -1;
	double average = threshold_percents[0];
	int total = 1;
	index = 1;
	while (index < samples) {
		if (total > desired_total) {
			found_threshold = min_threshold + threshold_sample_interval * (index - total / 2);
			break;
			
		} else if (fabs(threshold_percents[index] - average) > desired_deviation) {
			// If the difference between the next sample and the average is too large, reset.
			average = threshold_percents[index];
			total = 1;
			
		} else {
			// Adjust the running average.
			average = ((average * total) / (total + 1)) + (threshold_percents[index] / (total + 1));
			total++;
		}
		index++;
	}
	if (found_threshold == -1) {
		printf("WARNING: Could not find a stable threshold, backing up to hardcoded... \n");
		found_threshold = 750;
	}
	
	printf("Using threshold: %d \n", found_threshold);
	return found_threshold;
}

/**
 * Converts cycles to milliseconds.
 * If the clock speed hasn't been found, calculate it now.
 */
float cycles_to_ms(u_int64_t cycles) {
	static double MHz = -1;
	
	// If we haven't calculated MHz, do it now.
	if (MHz == -1) {
		struct timespec sleep_time;
		sleep_time.tv_nsec = 1000000;
		sleep_time.tv_sec = 0;
		
		// Want to find the k best measures for the sleep cycles.
		int i;
		int k = 25;
		int k_best_iterations[k] ;
		for (i = 0; i < k; i++) {
			k_best_iterations[i] = INT_MAX;
		}
		
		// Warm up cache.
		measure_sleep_cycles(&sleep_time);
		
		// Run iterations to measure sleep cycles multiple times.
		int iterations = 200;
		for (i = 0; i < iterations; i++) {
			int count = measure_sleep_cycles(&sleep_time);
			
			// Check our current k best, to see if this is better.
			int j;
			for (j = 0; j < k; j++) {
				if (count < k_best_iterations[j]) {
					
					// A better value was found, shift array and replace.
					int h;
					for (h = k-2; h >= j; h--) {
						k_best_iterations[h+1] = k_best_iterations[h];
					}
					k_best_iterations[j] = count;
					break;
				} 
			}
		}
		
		// Sum and average the k best values.
		int sum = 0;
		for (i = 0; i < k; i++) {
			sum += k_best_iterations[i];
		}
		
		MHz = (sum / k) / 1e3;
		printf("Found clock speed: %f MHz\n", MHz); 
	}

    return (float)(cycles / MHz) / 1e3;
}

/**
 * Measures the cycles for a sleep of the specified time.
 */
int measure_sleep_cycles(struct timespec *sleep_time) {
	int start, end;
	start = get_counter();
	nanosleep(sleep_time, NULL);
	end = get_counter();
	return end - start;
}
