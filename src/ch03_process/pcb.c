/*
 * pcb.c - Process Control Block management
 *
 * Covers:
 *   - PCB creation and initialisation
 *   - Process state transitions with validation
 *   - PCB pretty-printing
 */

#include "process.h"

/* ANSI dark-grey used for EXIT_STATE display */
#define ANSI_DARK_GRAY "\033[90m"

/* ------------------------------------------------------------------ */
/* State helpers                                                        */
/* ------------------------------------------------------------------ */

const char *process_state_to_string(ProcessState state) {
    switch (state) {
        case NEW:        return "NEW";
        case READY:      return "READY";
        case RUNNING:    return "RUNNING";
        case BLOCKED:    return "BLOCKED";
        case EXIT_STATE: return "EXIT";
        default:         return "UNKNOWN";
    }
}

/* Returns an ANSI colour code that gives each state a distinct colour. */
const char *process_state_color(ProcessState state) {
    switch (state) {
        case NEW:        return ANSI_BOLD;
        case READY:      return ANSI_YELLOW;
        case RUNNING:    return ANSI_GREEN;
        case BLOCKED:    return ANSI_RED;
        case EXIT_STATE: return ANSI_DARK_GRAY;
        default:         return ANSI_RESET;
    }
}

/* ------------------------------------------------------------------ */
/* PCB creation                                                         */
/* ------------------------------------------------------------------ */

PCB create_process(int pid, const char *name, int priority) {
    PCB p;
    p.pid             = pid;
    strncpy(p.name, name, sizeof(p.name) - 1);
    p.name[sizeof(p.name) - 1] = '\0';
    p.state           = NEW;
    p.priority        = priority;
    p.program_counter = 0x0000;
    p.memory_base     = pid * 0x1000;   /* each process gets a separate simulated page */
    p.memory_limit    = 0x4000;
    p.cpu_time        = 0;
    p.waiting_time    = 0;
    for (int i = 0; i < 4; i++)
        p.registers[i] = 0;
    return p;
}

/* ------------------------------------------------------------------ */
/* PCB display                                                          */
/* ------------------------------------------------------------------ */

void print_pcb(const PCB *p) {
    const char *color = process_state_color(p->state);

    printf("  [ PCB ]  PID=%-3d  Name=%-20s\n", p->pid, p->name);
    printf("  +-----------------------------------------+\n");
    printf("  | State      : %s%-8s%s   Priority : %-3d  |\n",
           color, process_state_to_string(p->state), ANSI_RESET, p->priority);
    printf("  | PC         : 0x%04X       CPU Time : %-5d |\n",
           p->program_counter, p->cpu_time);
    printf("  | Mem base   : 0x%04X       Limit    : 0x%04X |\n",
           p->memory_base, p->memory_limit);
    printf("  | Registers  : R0=%-4d  R1=%-4d  R2=%-4d  R3=%-4d\n",
           p->registers[0], p->registers[1], p->registers[2], p->registers[3]);
    printf("  | Wait time  : %-5d\n", p->waiting_time);
    printf("  +-----------------------------------------+\n");
}

/* ------------------------------------------------------------------ */
/* State machine                                                        */
/* ------------------------------------------------------------------ */

/*
 * Legal transitions enforced by the OS:
 *
 *   NEW     -> READY
 *   READY   -> RUNNING          (dispatched by scheduler)
 *   RUNNING -> READY            (preempted by timer interrupt)
 *   RUNNING -> BLOCKED          (voluntary I/O wait)
 *   RUNNING -> EXIT_STATE       (process terminates)
 *   BLOCKED -> READY            (I/O or event completed)
 */
static bool is_valid_transition(ProcessState from, ProcessState to) {
    switch (from) {
        case NEW:        return to == READY;
        case READY:      return to == RUNNING;
        case RUNNING:    return (to == READY || to == BLOCKED || to == EXIT_STATE);
        case BLOCKED:    return to == READY;
        case EXIT_STATE: return false;
        default:         return false;
    }
}

void change_process_state(PCB *p, ProcessState new_state) {
    if (!is_valid_transition(p->state, new_state)) {
        printf(ANSI_RED
               "  [ERROR] Illegal transition: %s -> %s  (PID %d '%s')\n"
               ANSI_RESET,
               process_state_to_string(p->state),
               process_state_to_string(new_state),
               p->pid, p->name);
        return;
    }

    printf("  [PID %d] %-14s  %s%-8s%s  -->  %s%-8s%s\n",
           p->pid,
           p->name,
           process_state_color(p->state),
           process_state_to_string(p->state),
           ANSI_RESET,
           process_state_color(new_state),
           process_state_to_string(new_state),
           ANSI_RESET);

    p->state = new_state;
}
