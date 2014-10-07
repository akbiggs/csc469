#ifndef TSC_H
#define TSC_H

#include <sys/types.h>

extern void start_counter();
extern u_int64_t get_counter();

#endif // TSC_H
