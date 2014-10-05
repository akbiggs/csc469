#ifndef TRACKER_H
#define TRACKER_H

u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples);
float cycles_to_ms(u_int64_t cycles);
int plot_samples(char* filename, u_int64_t* samples);

#endif // TRACKER_
