#ifndef PROCESS_H
#define PROCESS_H

#include "common.h"

typedef enum {
    PROC_NEW,
    PROC_READY,
    PROC_RUNNING,
    PROC_BLOCKED,
    PROC_TERMINATED
} ProcessState;

typedef struct {
    int          pid;
    char         name[MAX_NAME_LEN];
    ProcessState state;
    int          priority;
    int          burst_time;
    int          remaining_time;
    int          arrival_time;
    int          waiting_time;
    int          turnaround_time;
} PCB;

void demo_process_states(void);
void demo_process_creation(void);
void run_ch03_demo(void);

#endif /* PROCESS_H */
