#include "semaphore_sim.h"

void run_ch05_concurrency_demo(void) {
    demo_race_condition();
    press_enter_to_continue();

    demo_mutex_solution();
    press_enter_to_continue();

    demo_peterson();
    press_enter_to_continue();

    demo_semaphore();
    press_enter_to_continue();

    demo_producer_consumer();
    press_enter_to_continue();

    demo_readers_writers();
    press_enter_to_continue();
}

void run_ch05_demo(void) {
    print_header("CH05: Concurrency — Mutual Exclusion and Synchronization");
    run_ch05_concurrency_demo();
}
