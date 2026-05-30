/*
 * process_state_machine.c - Full lifecycle simulation for three processes
 *
 * State transition diagram:
 *
 *          admit
 *   [NEW] ------> [READY] <-----------+
 *                   |   ^             |
 *          dispatch |   | preempt     | I/O done
 *                   v   |             |
 *                [RUNNING] ---> [BLOCKED]
 *                   |
 *              exit |
 *                   v
 *                [EXIT]
 */

#include "process.h"

/* Helper: print a section label without clearing the screen */
static void section(const char *label) {
    printf("\n" ANSI_BOLD "  ── %s ──\n" ANSI_RESET, label);
    print_separator();
}

void simulate_process_state_transition(void) {
    print_header("Process State Machine — Full Lifecycle");

    printf("  Scenario: three processes compete for one CPU.\n");
    printf("  Watch how the OS moves each PCB through the state diagram.\n\n");

    /* ---- Process creation ---- */
    PCB p1 = create_process(1, "Process-Alpha", 3);
    PCB p2 = create_process(2, "Process-Beta",  2);
    PCB p3 = create_process(3, "Process-Gamma", 1);

    printf("  Created 3 processes (all start in NEW state):\n");
    print_pcb(&p1);
    print_pcb(&p2);
    print_pcb(&p3);

    /* ---- Phase 1: admission ---- */
    section("Phase 1  |  OS admits processes into the READY queue");
    printf("  (Analogy: job submitted to the OS and accepted for execution)\n\n");
    change_process_state(&p1, READY);
    change_process_state(&p2, READY);
    change_process_state(&p3, READY);

    /* ---- Phase 2: first dispatch ---- */
    section("Phase 2  |  Scheduler picks highest-priority process");
    printf("  Alpha has priority 3 — highest — so it is dispatched first.\n\n");
    change_process_state(&p1, RUNNING);
    p1.program_counter = 0x0100;
    p1.registers[0]    = 10;
    p1.registers[1]    = 20;

    /* ---- Phase 3: I/O request ---- */
    section("Phase 3  |  Alpha requests I/O  (RUNNING -> BLOCKED)");
    printf("  The CPU cannot busy-wait.  Alpha is moved to BLOCKED;\n");
    printf("  Beta (next highest priority) is dispatched instead.\n\n");
    change_process_state(&p1, BLOCKED);
    change_process_state(&p2, RUNNING);
    p2.program_counter = 0x0200;
    p2.registers[0]    = 30;

    /* ---- Phase 4: I/O completion ---- */
    section("Phase 4  |  Alpha's I/O completes  (BLOCKED -> READY)");
    printf("  The I/O interrupt handler moves Alpha back to READY.\n");
    printf("  Alpha does not immediately preempt Beta — it must wait.\n\n");
    change_process_state(&p1, READY);
    p1.waiting_time += 5;

    /* ---- Phase 5: preemption ---- */
    section("Phase 5  |  Beta's time quantum expires  (RUNNING -> READY)");
    printf("  The timer fires.  Beta is preempted; Alpha (higher priority)\n");
    printf("  is dispatched again.\n\n");
    change_process_state(&p2, READY);
    p2.cpu_time += 10;
    change_process_state(&p1, RUNNING);

    /* ---- Phase 6: completion ---- */
    section("Phase 6  |  Processes run to completion");
    p1.cpu_time += 15;
    change_process_state(&p1, EXIT_STATE);

    change_process_state(&p2, RUNNING);
    p2.cpu_time += 8;
    change_process_state(&p2, EXIT_STATE);

    change_process_state(&p3, RUNNING);
    p3.cpu_time += 4;
    change_process_state(&p3, EXIT_STATE);

    /* ---- Summary ---- */
    printf("\n" ANSI_BOLD "  Final PCB snapshots:\n" ANSI_RESET);
    print_separator();
    print_pcb(&p1);
    print_pcb(&p2);
    print_pcb(&p3);

    printf(ANSI_GREEN "\n  All processes have exited.  CPU is now idle.\n" ANSI_RESET);
}
