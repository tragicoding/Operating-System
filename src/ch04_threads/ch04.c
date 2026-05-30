#include "thread_sim.h"

void demo_user_threads(void) {
    print_header("User-Level Threads");
    printf("  Managed by user-space thread library\n");
    printf("  Kernel unaware of individual threads\n");
    printf("\n  [Full simulation coming soon]\n");
}

void demo_kernel_threads(void) {
    print_header("Kernel-Level Threads");
    printf("  Managed directly by the OS kernel\n");
    printf("  True parallelism on multi-core systems\n");
    printf("\n  [Full simulation coming soon]\n");
}

void run_ch04_demo(void) {
    print_header("CH04: Threads");
    printf("  Topics covered:\n");
    printf("  - Thread concept and benefits\n");
    printf("  - User-level vs kernel-level threads\n");
    printf("  - Multithreading models (M:1, 1:1, M:N)\n");
    printf("  - POSIX Pthreads overview\n");
    printf("\n  [Demo not yet implemented — coming soon]\n");
    press_enter_to_continue();
}
