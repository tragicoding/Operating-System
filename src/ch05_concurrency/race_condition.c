#include "semaphore_sim.h"

/* Each thread increments this shared variable ITERATIONS times. */
#define ITERATIONS  100000
#define NUM_THREADS 2

static long shared_counter;
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Unsafe increment — classic read-modify-write race */
static void *increment_unsafe(void *arg) {
    (void)arg;
    for (int i = 0; i < ITERATIONS; i++)
        shared_counter++;   /* not atomic: load, add, store */
    return NULL;
}

/* Safe increment — mutex protects the critical section */
static void *increment_safe(void *arg) {
    (void)arg;
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&counter_mutex);
        shared_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

void demo_race_condition(void) {
    print_header("Race Condition Demo (No Mutex)");
    printf("  %d threads each increment shared_counter %d times.\n",
           NUM_THREADS, ITERATIONS);
    printf("  Expected: " ANSI_BOLD "%d" ANSI_RESET "\n\n", NUM_THREADS * ITERATIONS);

    shared_counter = 0;
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment_unsafe, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    int expected = NUM_THREADS * ITERATIONS;
    int ok = (shared_counter == expected);
    printf("  Result WITHOUT mutex: %s%ld%s  %s\n",
           ok ? ANSI_GREEN : ANSI_RED,
           shared_counter,
           ANSI_RESET,
           ok ? "<-- lucky, try again" : "<-- DATA RACE! lost updates");
}

void demo_mutex_solution(void) {
    print_header("Mutex Solution");
    printf("  Same workload — every increment is inside pthread_mutex_lock/unlock.\n\n");

    shared_counter = 0;
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment_safe, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    int expected = NUM_THREADS * ITERATIONS;
    printf("  Result WITH mutex: " ANSI_GREEN "%ld" ANSI_RESET
           "  (expected %d) %s\n",
           shared_counter, expected,
           shared_counter == expected ? "<-- correct!" : "<-- BUG");
}
