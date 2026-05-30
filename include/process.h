#ifndef PROCESS_H
#define PROCESS_H

#include "common.h"

/*
 * Five fundamental process states.
 * EXIT_STATE is named to avoid collision with stdlib EXIT_SUCCESS / EXIT_FAILURE.
 */
typedef enum {
    NEW,        /* process created, not yet admitted to the ready queue  */
    READY,      /* in ready queue, waiting to be dispatched to the CPU   */
    RUNNING,    /* currently executing on the CPU                        */
    BLOCKED,    /* waiting for I/O or an event to complete               */
    EXIT_STATE  /* execution finished; resources being released          */
} ProcessState;

/*
 * Process Control Block (PCB)
 * The OS maintains one PCB per process.  It is the complete snapshot of
 * everything the OS needs to know about — and later restore — a process.
 */
typedef struct {
    int          pid;
    char         name[32];
    ProcessState state;
    int          priority;          /* higher value = higher priority          */
    int          program_counter;   /* address of the next instruction to run  */
    int          registers[4];      /* simulated CPU registers: R0 – R3        */
    int          memory_base;       /* start of this process's address space   */
    int          memory_limit;      /* size of this process's address space    */
    int          cpu_time;          /* total CPU ticks consumed so far         */
    int          waiting_time;      /* total ticks spent in the READY queue    */
} PCB;

/* ---- PCB utility functions (pcb.c) ---- */
const char *process_state_to_string(ProcessState state);
const char *process_state_color(ProcessState state);
PCB         create_process(int pid, const char *name, int priority);
void        print_pcb(const PCB *p);
void        change_process_state(PCB *p, ProcessState new_state);

/* ---- Demo entry points ---- */
void simulate_process_state_transition(void); /* process_state_machine.c */
void simulate_context_switch(void);           /* dispatcher.c            */
void run_ch03_process_demo(void);             /* dispatcher.c            */

#endif /* PROCESS_H */
