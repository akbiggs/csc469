#include "tracker.h"

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

	printf("== TRACKING ACTIVITY ==\n");
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

    if (plot_samples("runplot.sh", samples, num_samples, cycles_to_ms(start))) {
        fprintf(stderr, "Failed to write samples to a gnuplot file.\n");
    }

    return 0;
}

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
 * Converts cycles to milliseconds.
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

int measure_sleep_cycles(struct timespec *sleep_time) {
	int start, end;
	start = get_counter();
	nanosleep(sleep_time, NULL);
	end = get_counter();
	return end - start;
}

/**
 * Finds a good threshold by iteratively testing a range of thresholds
 * in order to graph the average number of inactive periods found per
 * iteration. It then makes a selection from a found threshold range
 * that is long and varies little.
 */
int find_threshold(int num) {
	
	// Set the parameters for the threshold sampling.
	int min_threshold = 50;
	int max_threshold = 3000;
	int threshold_sample_interval = 30;
	
	printf("== FINDING THRESHOLD ==\nFinding the ideal threshold from %d to %d.\n", min_threshold, max_threshold);
	
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
