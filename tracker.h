#ifndef TRACKER_H
#define TRACKER_H

#include <sys/types.h>

int plot_samples(char* filename, u_int64_t* samples, int num_samples, float start_ms);

#endif // TRACKER_H
