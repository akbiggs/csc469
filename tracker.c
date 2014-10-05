#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "tracker.h"
#include "tsc.h"

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

float cycles_to_ms(u_int64_t cycles) {
    // TODO: Experiment to figure out how these actually relate.
    return (float)(cycles / 1000.0f);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_samples> <threshold>\n", argv[0]);
        return -1;
    }

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

    }

    if (plot_samples("what.eps", samples)) {
        fprintf(stderr, "Failed to write samples to a gnuplot file.\n");
    }

    return 0;
}

int plot_samples(char* filename, u_int64_t* samples) {
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

    int millis_elapsed = 0;
    if (sizeof(samples) != 0) {
        
        int num_samples = sizeof(samples) / sizeof(samples[0]);
        printf("Number of samples %d\n", num_samples);
        int cumulative_millis = 0;
        int i;
        for (i = 0; i < num_samples; i++) {
            u_int64_t start = samples[i * 2];
            u_int64_t end = samples[i * 2 + 1];
            
            printf("%d\n", i);
            
            char* color;
            if (i % 2 == 0) {
                color = "blue"; // active
            } else {
                color = "red"; // inactive
            }
            
            cumulative_millis += cycles_to_ms(end);
            
            while (millis_elapsed < cumulative_millis) {
                fprintf(plot_file, "set object %d rect from %d, 1 to %d, 2 fc rgb \"%s\" fs solid\n",
                        i + 1, millis_elapsed, millis_elapsed + 1, color);
                millis_elapsed++;
            }
        }
    }

    fprintf(plot_file, "plot [0:%d] [0:3] 0\n", millis_elapsed);
    fputs("---EOF---\n", plot_file);

    fclose(plot_file);

    return 0;
}