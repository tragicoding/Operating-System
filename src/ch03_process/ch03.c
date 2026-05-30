#include "process.h"
#include "scheduler.h"

void demo_process_states(void) {
    print_header("Process State Transitions");
    printf("  new -> ready -> running -> terminated\n");
    printf("         ready <- blocked <- running\n");
    printf("\n  [Full simulation coming soon]\n");
}

void demo_process_creation(void) {
    print_header("Process Creation (fork/exec model)");
    printf("  Parent creates child process\n");
    printf("  Child inherits address space\n");
    printf("\n  [Full simulation coming soon]\n");
}

void run_ch03_demo(void) {
    print_header("CH03: Process Description and Control");
    printf("  Topics covered:\n");
    printf("  - Process concept and PCB\n");
    printf("  - Process state transitions\n");
    printf("  - Process creation and termination\n");
    printf("  - CPU scheduling (FCFS, SJF, RR, Priority)\n");
    printf("\n  [Demo not yet implemented — coming soon]\n");
    press_enter_to_continue();
}
