#include "deadlock.h"

void run_ch06_deadlock_demo(void) {
    demo_deadlock_concept();
    press_enter_to_continue();

    demo_rag();
    press_enter_to_continue();

    demo_bankers_algorithm();
    press_enter_to_continue();

    demo_dining_philosophers();
    press_enter_to_continue();
}

void run_ch06_demo(void) {
    print_header("CH06: Deadlock and Starvation");
    run_ch06_deadlock_demo();
}
