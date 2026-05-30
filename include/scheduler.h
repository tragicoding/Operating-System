#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

typedef enum {
    SCHED_FCFS,
    SCHED_SJF,
    SCHED_ROUND_ROBIN,
    SCHED_PRIORITY
} SchedulerType;

void demo_fcfs(void);
void demo_round_robin(void);
void demo_sjf(void);

#endif /* SCHEDULER_H */
