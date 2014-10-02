#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "tracker.h"
#include "tsc.h"

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples) {
    u_int64_t start_active = get_counter();

    u_int64_t prev_counter = start_active;
    u_int64_t cur_counter = start_active;

    // BUG: Two samples are always taken at once using this logic for some
    // reason. Is that expected behavior?
    
    int num_sampled = 0;
    while (num_sampled < num) {
        while (cur_counter - prev_counter <= threshold) {
            prev_counter = cur_counter;
            cur_counter = get_counter();
        }

        printf("adding sample %d\n", num_sampled);

        samples[num_sampled*2] = prev_counter;
        samples[num_sampled*2 + 1] = cur_counter;

        prev_counter = cur_counter;
        cur_counter = get_counter();

        num_sampled++;
    }

    return start_active;
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

    int num_samples = atoi(argv[1]);
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

    return 0;
}
