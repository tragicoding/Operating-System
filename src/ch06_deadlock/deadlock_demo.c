#include "deadlock.h"

/*
 * Deadlock Concept Demo
 *
 * Shows the classic two-resource circular-wait scenario safely using
 * pthread_mutex_trylock.  An atomic counter synchronises both threads so
 * they both hold one resource before either attempts the other, reproducing
 * the exact deadlock position — then both detect the blockage and release.
 *
 * The program never hangs.
 */

static pthread_mutex_t res_A = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t res_B = PTHREAD_MUTEX_INITIALIZER;

/* Counts how many threads have acquired their first resource */
static atomic_int both_ready = 0;

/* ── Thread A: wants A then B ── */
static void *thread_a(void *arg) {
    (void)arg;

    pthread_mutex_lock(&res_A);
    printf("  Thread-A: acquired Resource-A\n");
    atomic_fetch_add(&both_ready, 1);

    /* Spin until Thread-B has also taken Resource-B:
     * both threads are now holding one lock each — the classic deadlock state */
    while (atomic_load(&both_ready) < 2)
        ;

    /* Try for Resource-B — Thread-B holds it, so this will fail */
    if (pthread_mutex_trylock(&res_B) == 0) {
        printf("  Thread-A: acquired Resource-B (no deadlock this run)\n");
        pthread_mutex_unlock(&res_B);
    } else {
        printf("  Thread-A: Resource-B BLOCKED"
               " — holds A, needs B  ← CIRCULAR WAIT\n");
    }

    printf("  Thread-A: releasing Resource-A (resolution: abort / timeout)\n");
    pthread_mutex_unlock(&res_A);
    return NULL;
}

/* ── Thread B: wants B then A ── */
static void *thread_b(void *arg) {
    (void)arg;

    pthread_mutex_lock(&res_B);
    printf("  Thread-B: acquired Resource-B\n");
    atomic_fetch_add(&both_ready, 1);

    while (atomic_load(&both_ready) < 2)
        ;

    if (pthread_mutex_trylock(&res_A) == 0) {
        printf("  Thread-B: acquired Resource-A (no deadlock this run)\n");
        pthread_mutex_unlock(&res_A);
    } else {
        printf("  Thread-B: Resource-A BLOCKED"
               " — holds B, needs A  ← CIRCULAR WAIT\n");
    }

    printf("  Thread-B: releasing Resource-B (resolution: abort / timeout)\n");
    pthread_mutex_unlock(&res_B);
    return NULL;
}

void demo_deadlock_concept(void) {
    print_header("Deadlock Concept Demo");

    printf("  Four Coffman conditions — ALL must hold for deadlock:\n");
    printf("  1. Mutual Exclusion  — only one thread can hold a resource\n");
    printf("  2. Hold and Wait     — thread holds one resource while waiting\n");
    printf("  3. No Preemption     — resources cannot be forcibly taken\n");
    printf("  4. Circular Wait     — P0 waits for P1, P1 waits for P0, ...\n\n");

    printf("  Live demo — Thread-A (A→B) vs Thread-B (B→A):\n");
    printf("  (trylock is used so the program never hangs)\n\n");

    atomic_store(&both_ready, 0);

    pthread_t ta, tb;
    pthread_create(&ta, NULL, thread_a, NULL);
    pthread_create(&tb, NULL, thread_b, NULL);
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    printf("\n  Both threads resolved — no permanent hang.\n");
    printf("  Without trylock, this would be an infinite deadlock.\n");
}
