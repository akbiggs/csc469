#ifndef TRACKER_H
#define TRACKER_H

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples);
float cycles_to_ms(u_int64_t cycles);
void find_threshold();

#endif // TRACKER_H
