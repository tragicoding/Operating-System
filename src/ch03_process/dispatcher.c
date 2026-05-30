/*
 * dispatcher.c - Dispatcher and context switch simulation
 *
 * The dispatcher is the low-level OS mechanism that:
 *   1. Saves the CPU registers / program counter into the outgoing PCB.
 *   2. Restores the CPU registers / program counter from the incoming PCB.
 *   3. Transfers control to the new process.
 *
 * Context-switch time is pure overhead — the system does no useful work
 * during a switch.  Reducing its frequency (larger time quanta) or its
 * duration (hardware TLB flush optimisations) is a key OS design goal.
 */

#include "process.h"
#include "scheduler.h"

/* ------------------------------------------------------------------ */
/* Dispatcher implementation                                            */
/* ------------------------------------------------------------------ */

void dispatcher_init(Dispatcher *d) {
    d->running          = NULL;
    d->context_switches = 0;
}

/*
 * dispatcher_switch()
 *
 * Precondition: the caller has already set the outgoing process's state
 * (e.g. BLOCKED or READY via change_process_state).
 *
 * This function:
 *   - Prints the SAVE step  (outgoing PCB snapshot)
 *   - Prints the LOAD step  (incoming PCB snapshot)
 *   - Calls change_process_state(next, RUNNING)
 *   - Updates d->running
 */
void dispatcher_switch(Dispatcher *d, PCB *next) {
    d->context_switches++;
    printf("  " ANSI_BOLD "┌─ Context Switch #%d " ANSI_RESET
           "──────────────────────────────────\n",
           d->context_switches);

    /* Step 1 — save outgoing context */
    if (d->running != NULL) {
        PCB *prev = d->running;
        printf("  │ " ANSI_YELLOW "[SAVE]" ANSI_RESET
               "  PID=%-3d %-14s  PC=0x%04X  "
               "R0=%-4d R1=%-4d R2=%-4d R3=%-4d\n",
               prev->pid, prev->name,
               prev->program_counter,
               prev->registers[0], prev->registers[1],
               prev->registers[2], prev->registers[3]);
    } else {
        printf("  │ " ANSI_YELLOW "[SAVE]" ANSI_RESET "  (CPU was idle — nothing to save)\n");
    }

    /* Step 2 — load incoming context */
    printf("  │ " ANSI_GREEN "[LOAD]" ANSI_RESET
           "  PID=%-3d %-14s  PC=0x%04X  "
           "R0=%-4d R1=%-4d R2=%-4d R3=%-4d\n",
           next->pid, next->name,
           next->program_counter,
           next->registers[0], next->registers[1],
           next->registers[2], next->registers[3]);

    /* Step 3 — hand CPU to the new process */
    d->running = next;
    printf("  └─────────────────────────────────────────────────────\n");
    change_process_state(next, RUNNING);
    printf("\n");
}

/* ------------------------------------------------------------------ */
/* Context switch demo                                                  */
/* ------------------------------------------------------------------ */

void simulate_context_switch(void) {
    print_header("Dispatcher and Context Switch");

    printf("  Two processes share one CPU.\n");
    printf("  Observe how the OS saves and restores CPU state on each switch.\n\n");

    Dispatcher cpu;
    dispatcher_init(&cpu);

    PCB editor   = create_process(101, "Editor",   2);
    PCB compiler = create_process(102, "Compiler", 3);

    /* Give each process a distinct saved CPU state */
    editor.state           = READY;
    editor.program_counter = 0x1A00;
    editor.registers[0]    = 7;
    editor.registers[1]    = 14;

    compiler.state           = READY;
    compiler.program_counter = 0x2B00;
    compiler.registers[0]    = 99;
    compiler.registers[2]    = 42;

    printf(ANSI_BOLD "  Initial PCB snapshots:\n" ANSI_RESET);
    print_separator();
    print_pcb(&editor);
    print_pcb(&compiler);

    /* ---- Switch 1: idle -> Compiler (higher priority) ---- */
    printf(ANSI_BOLD "\n  [Event] CPU idle. Schedule highest-priority process.\n" ANSI_RESET);
    dispatcher_switch(&cpu, &compiler);

    /* Simulate Compiler running for a while */
    compiler.program_counter += 0x18;
    compiler.registers[0]     = 55;
    compiler.cpu_time         += 8;

    /* ---- Switch 2: Compiler blocks for I/O -> Editor ---- */
    printf(ANSI_BOLD "  [Event] Compiler requests disk I/O — must block.\n" ANSI_RESET);
    change_process_state(&compiler, BLOCKED);
    dispatcher_switch(&cpu, &editor);

    /* Simulate Editor running */
    editor.program_counter += 0x0C;
    editor.registers[1]     = 28;
    editor.cpu_time         += 5;

    /* ---- Switch 3: Compiler I/O done -> preempts Editor ---- */
    printf(ANSI_BOLD "  [Event] Compiler I/O complete. Higher-priority; preempts Editor.\n" ANSI_RESET);
    change_process_state(&compiler, READY); /* I/O done — BLOCKED -> READY         */
    change_process_state(&editor, READY);   /* preempted by higher-priority process */
    editor.waiting_time += 3;
    dispatcher_switch(&cpu, &compiler);

    /* ---- Final state ---- */
    printf(ANSI_BOLD "  PCB snapshots after %d context switches:\n" ANSI_RESET,
           cpu.context_switches);
    print_separator();
    print_pcb(&editor);
    print_pcb(&compiler);

    printf(ANSI_GREEN
           "\n  Total context switches: %d\n"
           "  Note: context-switch overhead grows with switch frequency.\n"
           ANSI_RESET,
           cpu.context_switches);
}

/* ------------------------------------------------------------------ */
/* CH03 sub-menu                                                        */
/* ------------------------------------------------------------------ */

void run_ch03_process_demo(void) {
    int choice;

    while (1) {
        clear_screen();
        printf(ANSI_BOLD ANSI_CYAN);
        printf("============================================================\n");
        printf("     CH03: Process Description and Control\n");
        printf("============================================================\n");
        printf(ANSI_RESET "\n");
        printf("  " ANSI_BOLD "[1]" ANSI_RESET " Process State Machine  (NEW -> READY -> RUNNING -> ... -> EXIT)\n");
        printf("  " ANSI_BOLD "[2]" ANSI_RESET " Dispatcher & Context Switch  (save / restore CPU state)\n");
        printf("  " ANSI_BOLD "[3]" ANSI_RESET " Inspect PCB Structure\n");
        printf("  " ANSI_BOLD "[0]" ANSI_RESET " Back to Main Menu\n");
        printf("\n  Select: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        switch (choice) {
            case 1:
                clear_screen();
                simulate_process_state_transition();
                press_enter_to_continue();
                break;

            case 2:
                clear_screen();
                simulate_context_switch();
                press_enter_to_continue();
                break;

            case 3: {
                clear_screen();
                print_header("PCB Structure Inspection");
                printf("  A freshly created process — all fields at their initial values:\n\n");
                PCB sample = create_process(42, "SampleProc", 5);
                print_pcb(&sample);

                printf("\n  Advancing through NEW -> READY -> RUNNING:\n\n");
                change_process_state(&sample, READY);
                sample.program_counter = 0xBEEF;
                sample.registers[0] = 1;
                sample.registers[1] = 2;
                sample.registers[2] = 3;
                sample.registers[3] = 4;
                change_process_state(&sample, RUNNING);
                sample.cpu_time    = 37;
                sample.waiting_time = 12;

                printf("\n  PCB after running for a while:\n\n");
                print_pcb(&sample);
                press_enter_to_continue();
                break;
            }

            case 0:
                return;

            default:
                printf(ANSI_RED "  Invalid option.\n" ANSI_RESET);
                press_enter_to_continue();
                break;
        }
    }
}
