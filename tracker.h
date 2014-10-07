#ifndef TRACKER_H
#define TRACKER_H

#include <sys/types.h>

int plot_samples(char* filename, u_int64_t* samples, int num_samples, float start_ms);

void test_cache_miss_time();
void increment_array(int** array, int increments, int max_array_index_select);

#endif // TRACKER_H
