#ifndef THREAD_SIM_H
#define THREAD_SIM_H

#include "common.h"

typedef enum {
    THREAD_NEW,
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_TERMINATED
} ThreadState;

typedef struct {
    int         tid;
    int         pid;
    char        name[MAX_NAME_LEN];
    ThreadState state;
} TCB;

void demo_user_threads(void);
void demo_kernel_threads(void);
void run_ch04_demo(void);

#endif /* THREAD_SIM_H */
