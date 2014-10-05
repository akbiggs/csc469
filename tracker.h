#ifndef TRACKER_H
#define TRACKER_H

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples);
float cycles_to_ms(u_int64_t cycles);

void test_cache_miss_time();
void increment_array(int** array, int increments, int max_array_index_select);
void find_threshold();

#endif // TRACKER_H
