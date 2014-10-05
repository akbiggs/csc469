#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#include "tracker.h"
#include "tsc.h"

// TODO: figure out gnuplot
// TODO: write run_test_experiment_A bash file
// TODO: write the threshold finder code
// TODO: do clock to ms
// TOOD?: Understand how proc/interupts matters
// TODO?: Understand ratio argument

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples) {
    u_int64_t start_active = get_counter();

    u_int64_t prev_counter = start_active;
    u_int64_t cur_counter = start_active;
    
    int num_sampled = 0;
    while (num_sampled < num) {

        while (cur_counter - prev_counter <= threshold) {
            prev_counter = cur_counter;
            cur_counter = get_counter();
        }

        printf("adding sample %d\n", num_sampled);

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

void find_threshold() { 
	int iterations = 1000000;
	int max_threshold = 200;
	int min_threshold = 50;
	int threshold_sample_interval = 1;
	
	int size = 10000;
	int samples[size];

	int a;
	for (a = 0; a < 100; a++) {
		u_int64_t prev_counter = get_counter();
		samples[1] = 1;
		u_int64_t cur_counter = get_counter();
		printf("[1] -- %d\n", cur_counter - prev_counter);
	}
	srand(time(NULL));
	for (a = 0; a < 100; a++) {
		int a = rand() % size;
		int b = rand() % size;
		int c = rand() % size;
		int d = rand() % size;
		u_int64_t prev_counter = get_counter();
		samples[a] = 2;
		u_int64_t cur_counter = get_counter();
		printf("[%d] -- %d\n", a, cur_counter - prev_counter);
	}
	
	int threshold;
	for (threshold = min_threshold; threshold < max_threshold; threshold += threshold_sample_interval) {
		u_int64_t cur_counter = get_counter();
		u_int64_t prev_counter = cur_counter;
		
		
		int inactive_count = 0;
		int i = 0;
		int sum_diff = 0;
		for (i = 0; i < iterations; i++) {
			cur_counter = get_counter();
			sum_diff += cur_counter - prev_counter;
			if (cur_counter - prev_counter > threshold) {
				inactive_count++;
			}
			prev_counter = cur_counter;
		}
		int average_diff = sum_diff / iterations;
		printf("threshold %d: %d inactive periods (average_diff = %d / %d = %d)\n", threshold, inactive_count, sum_diff, iterations, average_diff);
		
	}
}

float cycles_to_ms(u_int64_t cycles) {
    // TODO: Experiment to figure out how these actually relate.
    return (float)(cycles / 1000.0f);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <num_samples> <threshold>\n", argv[0]);
        return -1;
    }
    
    find_threshold();

    int num_samples = atoi(argv[1]);
    // TODO: need to figure out how to determine the threshold experimentally
    int threshold = atoi(argv[2]);
    u_int64_t samples[num_samples*2];

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
       // TODO: Plot this shit.
    }

    return 0;
}
