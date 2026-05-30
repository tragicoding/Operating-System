#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

typedef enum {
    SCHED_FCFS,
    SCHED_SJF,
    SCHED_ROUND_ROBIN,
    SCHED_PRIORITY
} SchedulerType;

/*
 * Dispatcher
 * The dispatcher is the OS module that hands CPU control to the process
 * selected by the scheduler.  Its main responsibilities:
 *   1. Save the context of the outgoing process into its PCB.
 *   2. Load the context of the incoming process from its PCB.
 *   3. Switch to user mode and jump to the new process's PC.
 */
typedef struct {
    PCB *running;           /* pointer to the currently running PCB (NULL = idle) */
    int  context_switches;  /* total context switches performed                   */
} Dispatcher;

void dispatcher_init(Dispatcher *d);
void dispatcher_switch(Dispatcher *d, PCB *next);

/* Placeholder scheduler demos (implemented in later iterations) */
void demo_fcfs(void);
void demo_round_robin(void);
void demo_sjf(void);

#endif /* SCHEDULER_H */
