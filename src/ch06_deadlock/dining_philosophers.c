#include "deadlock.h"

/*
 * Dining Philosophers Problem (E.W. Dijkstra, 1965)
 *
 * 5 philosophers sit at a round table with 5 forks.
 * Each needs both the left and right fork to eat.
 *
 * NAIVE approach (not run — would deadlock):
 *   All philosophers pick left fork simultaneously → all wait for right fork
 *   → circular wait → deadlock.
 *
 * SAFE approach — resource ordering:
 *   Always acquire the lower-numbered fork first.
 *   Philosopher 4 (forks 4 & 0) picks fork-0 THEN fork-4 instead of 4 then 0.
 *   This breaks the circular-wait condition because P0 and P4 now compete
 *   for fork-0, so one of them must block — the chain of circular dependency
 *   cannot form.
 */

#define N_PHILO    5
#define EAT_TIMES  3

static pthread_mutex_t forks[N_PHILO];
static pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

static void *philosopher(void *arg) {
    int id    = *(int *)arg;
    int left  = id;
    int right = (id + 1) % N_PHILO;

    /* Resource ordering: always pick the lower-numbered fork first */
    int first  = (left < right) ? left  : right;
    int second = (left < right) ? right : left;

    for (int i = 0; i < EAT_TIMES; i++) {
        /* Think */
        pthread_mutex_lock(&print_lock);
        printf("  Philosopher-%d thinking\n", id);
        pthread_mutex_unlock(&print_lock);

        /* Pick up forks in safe order */
        pthread_mutex_lock(&forks[first]);
        pthread_mutex_lock(&forks[second]);

        /* Eat */
        pthread_mutex_lock(&print_lock);
        printf("  Philosopher-%d eating"
               " (forks %d & %d)  [pick order: %d then %d]\n",
               id, left, right, first, second);
        pthread_mutex_unlock(&print_lock);

        /* Put down forks (reverse order — not required but tidy) */
        pthread_mutex_unlock(&forks[second]);
        pthread_mutex_unlock(&forks[first]);
    }

    pthread_mutex_lock(&print_lock);
    printf("  Philosopher-%d finished.\n", id);
    pthread_mutex_unlock(&print_lock);

    return NULL;
}

void demo_dining_philosophers(void) {
    print_header("Dining Philosophers Problem");

    printf("  5 philosophers — 5 forks — each needs 2 adjacent forks to eat.\n\n");

    printf("  [NAIVE — would DEADLOCK]\n");
    printf("  Each philosopher picks left fork first:\n");
    printf("    P0 holds fork-0, P1 holds fork-1, ..., P4 holds fork-4\n");
    printf("    Everyone waits for right fork → circular wait → DEADLOCK.\n\n");

    printf("  [SAFE — resource (fork) ordering]\n");
    printf("  Rule: always acquire the lower-numbered fork first.\n");
    printf("    P0 picks fork-0 then fork-1  (left < right — no change)\n");
    printf("    P4 picks fork-0 then fork-4  (right=0 < left=4 — REVERSED!)\n");
    printf("  P0 and P4 now both compete for fork-0 first.\n");
    printf("  One of them must wait → circular wait is broken.\n\n");

    printf("  Running safe version (%d philosophers × %d meals):\n\n",
           N_PHILO, EAT_TIMES);

    for (int i = 0; i < N_PHILO; i++)
        pthread_mutex_init(&forks[i], NULL);

    pthread_t threads[N_PHILO];
    int ids[N_PHILO];
    for (int i = 0; i < N_PHILO; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }
    for (int i = 0; i < N_PHILO; i++)
        pthread_join(threads[i], NULL);

    for (int i = 0; i < N_PHILO; i++)
        pthread_mutex_destroy(&forks[i]);

    printf("\n  " ANSI_GREEN "All philosophers finished — no deadlock." ANSI_RESET "\n");
}
