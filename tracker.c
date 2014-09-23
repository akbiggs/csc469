#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "tracker.h"
#include "tsc.h"

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples) {
    return get_counter();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <num_samples> <threshold>\n", argv[0]);
        return -1;
    }

    int num_samples = atoi(argv[1]);
    u_int64_t samples[num_samples];

    start_counter();

    while (1) {
        printf("%" PRIu64 "\n", inactive_periods(5000, 300, samples));
    }

    return 0;
}
