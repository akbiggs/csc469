#ifndef FORKER_H
#define FORKER_H

void run_child(char* name, int num_samples, int* pipe, int threshold, u_int64_t* samples);
void run_parent(int num_samples, int* first_pipe, int* second_pipe);

#endif // FORKER_H
