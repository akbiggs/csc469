#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples);
int wait_until_inactive_period(int threshold, u_int64_t* prev_counter, u_int64_t* cur_counter);

int find_threshold(int num);

float cycles_to_ms(u_int64_t cycles);
int measure_sleep_cycles(struct timespec *sleep_time);

#endif // COMMON_H
