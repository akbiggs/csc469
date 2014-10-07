#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

#include "tsc.h"
#include "common.h"
#include "forker.h"


int main(int argc, char** argv) {
	if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_samples>\n", argv[0]);
        return -1;
    }

    int num_samples = atoi(argv[1]);
    u_int64_t samples[num_samples * 2];
    int threshold = find_threshold(5);
    start_counter();
    
    int first_pipe[2];
    pipe(first_pipe);

    // fork two children processes to run
    switch (fork()) {
        case -1:
            fprintf(stderr, "Could not fork first child.\n");
            return 1;
        case 0:
            // in the first child
            close(first_pipe[0]); // close reader
            run_child("first", num_samples, first_pipe, threshold, samples);
            close(first_pipe[1]); // close writer
            break;

        default:
			// in the parent
			close(first_pipe[1]); // close writer
			
			int second_pipe[2];
			pipe(second_pipe);
            
            switch (fork()) {
                case -1:
                    fprintf(stderr, "Could not fork second child.\n");
                case 0:
                    // in the second child
                    close(first_pipe[0]); // close reader
					run_child("secnd", num_samples, second_pipe, threshold, samples);
					close(first_pipe[1]); // close writer
                    break;
                default:
                    // still in the parent
                    close(second_pipe[1]);  // close writer
                    
                    run_parent(num_samples, first_pipe, second_pipe);
                    close(first_pipe[0]);  // close reader
                    close(second_pipe[0]);  // close reader
                    break;
            }

            break;
    }

    return 0;
}

void run_parent(int num_samples, int* first_pipe, int* second_pipe) {
	
	u_int64_t first_read_buffer[num_samples * 2];
	u_int64_t first_start;
	read(first_pipe[0], &first_start, sizeof(u_int64_t));
	read(first_pipe[0], first_read_buffer, sizeof(u_int64_t) * num_samples * 2);
	
	u_int64_t second_read_buffer[num_samples * 2];
	u_int64_t second_start;
	read(second_pipe[0], &second_start, sizeof(u_int64_t));
	read(second_pipe[0], second_read_buffer, sizeof(u_int64_t) * num_samples * 2);
	
	int i;
	printf("first start %" PRIu64 " !!!\n", first_start);
	for (i = 0; i < num_samples * 2; i++) {
		printf("1st: parent sees %" PRIu64 " !!!\n", first_read_buffer[i]);
	}
	printf("second start %" PRIu64 " !!!\n", second_start);
	for (i = 0; i < num_samples * 2; i++) {
		printf("2nd: parent sees %" PRIu64 " !!!\n", second_read_buffer[i]);
	}
}

void run_child(char* name, int num_samples, int* pipe, int threshold, u_int64_t* samples) {	
	u_int64_t start = inactive_periods(num_samples, threshold, samples);
	
	int i;
	printf("%s first start %" PRIu64 " !!!\n", name, start);
	for (i = 0; i < num_samples * 2; i++) {
		printf("%s sees %" PRIu64 " !!!\n", name, samples[i]);
	}
	write(pipe[1], &start, sizeof(u_int64_t));
	write(pipe[1], samples, sizeof(u_int64_t) * num_samples * 2);
}
