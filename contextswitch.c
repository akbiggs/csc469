#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <inttypes.h>
#include <math.h>

#include "tsc.h"

int main(int argc, char** argv) {
    start_counter();
    int i;
    int j;
    switch (fork()) {
        case -1:
            fprintf(stderr, "Could not fork first child.\n");
            return 1;
        case 0:
            // in the first child
            for (i = 0; i < 500; i++) {
                printf("first: %" PRIu64 "\n", get_counter());
                for (j = 0; j < 10000000; j++) {
                    sqrt(7);
                }
            }
            break;

        default:
            // in the parent
            switch (fork()) {
                case -1:
                    fprintf(stderr, "Could not fork second child.\n");
                    
                case 0:
                    // in the second child
                    for (i = 0; i < 500; i++) {
                        printf("secon: %" PRIu64 "\n", get_counter());
                        for (j = 0; j < 10000000; j++) {
                            sqrt(7);
                        }
                    }
                    break;
                default:
                    // still in the parent
                    break;
            }

            break;
    }

    return 0;
}
