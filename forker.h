#ifndef FORKER_H
#define FORKER_H

void run_child(char* name, int num_samples, int* pipe, int threshold, u_int64_t* samples);
void run_parent(int num_samples, int* first_pipe, int* second_pipe);

int plot_samples(char* filename, u_int64_t* first_samples, u_int64_t* second_samples, int num_samples, float start_first, float start_second);

u_int64_t detect_context_switch_periods(int num, u_int64_t *samples);
int detect_context_switch(u_int64_t* prev_counter, u_int64_t* cur_counter);

#endif // FORKER_H
