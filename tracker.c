#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>

#include "tracker.h"
#include "common.h"
#include "tsc.h"

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
