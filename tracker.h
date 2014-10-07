#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#include "tsc.h"

#ifndef TRACKER_H
#define TRACKER_H

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples);
float cycles_to_ms(u_int64_t cycles);
int measure_sleep_cycles(struct timespec *sleep_time);

int plot_samples(char* filename, u_int64_t* samples, int num_samples, float start_ms);
int find_threshold(int num);
int wait_until_inactive_period(int threshold, u_int64_t* prev_counter, u_int64_t* cur_counter);

void test_cache_miss_time();
void increment_array(int** array, int increments, int max_array_index_select);

#endif // TRACKER_
