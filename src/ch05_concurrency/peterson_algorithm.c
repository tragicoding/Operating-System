#include "semaphore_sim.h"
#include <stdatomic.h>

/*
 * Peterson's Algorithm (1981) — software-only mutual exclusion for 2 threads.
 *
 * Each thread signals its intent with flag[i]=1, then politely yields with
 * turn = other_thread.  The busy-wait loop exits only when the other thread
 * has no intent (flag[j]==0) OR when it is this thread's turn.
 *
 * C11 atomic_int gives sequential-consistency needed on modern CPUs; plain
 * volatile is insufficient because compilers and CPUs may reorder stores.
 */

#define PETERSON_LOOPS 5

static atomic_int pet_flag[2];  /* flag[i]=1 means thread i wants to enter CS */
static atomic_int pet_turn;     /* tie-breaker: whose turn when both want in   */
static int        pet_counter;  /* the shared resource protected by Peterson   */

static pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

static void *peterson_thread(void *arg) {
    int me    = *(int *)arg;
    int other = 1 - me;

    for (int i = 0; i < PETERSON_LOOPS; i++) {
        /* ── Entry protocol ── */
        atomic_store(&pet_flag[me], 1);   /* "I want to enter" */
        atomic_store(&pet_turn, other);   /* "But you go first" */

        /* Busy-wait: spin while the other thread also wants in AND it is
         * the other thread's turn.  Sequential-consistency load ensures we
         * see the most recent values written by the other thread. */
        while (atomic_load(&pet_flag[other]) && atomic_load(&pet_turn) == other)
            ;   /* busy-waiting — CPU spins here */

        /* ── Critical section ── */
        int tmp = pet_counter;
        pthread_mutex_lock(&print_lock);
        printf("  Thread-%d [CS entry] counter = %d\n", me, tmp);
        pthread_mutex_unlock(&print_lock);

        pet_counter = tmp + 1;

        pthread_mutex_lock(&print_lock);
        printf("  Thread-%d [CS exit ] counter = %d\n", me, pet_counter);
        pthread_mutex_unlock(&print_lock);

        /* ── Exit protocol ── */
        atomic_store(&pet_flag[me], 0);   /* "I'm done, others may enter" */
    }
    return NULL;
}

void demo_peterson(void) {
    print_header("Peterson's Algorithm");
    printf("  Software mutual exclusion for 2 threads — no hardware atomics required.\n");
    printf("  Uses flag[2] (intent) and turn (tie-breaker).\n");
    printf("  Each thread runs the critical section %d times.\n\n", PETERSON_LOOPS);

    atomic_store(&pet_flag[0], 0);
    atomic_store(&pet_flag[1], 0);
    atomic_store(&pet_turn, 0);
    pet_counter = 0;

    pthread_t t0, t1;
    int id0 = 0, id1 = 1;
    pthread_create(&t0, NULL, peterson_thread, &id0);
    pthread_create(&t1, NULL, peterson_thread, &id1);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    int expected = PETERSON_LOOPS * 2;
    printf("\n  Final counter (expected %d): %s%d%s  %s\n",
           expected,
           pet_counter == expected ? ANSI_GREEN : ANSI_RED,
           pet_counter,
           ANSI_RESET,
           pet_counter == expected ? "<-- mutual exclusion held!" : "<-- BUG");
}
