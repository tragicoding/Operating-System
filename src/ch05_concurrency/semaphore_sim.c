#include "semaphore_sim.h"

/* ────────────────────────────────────────────
 * Simple logical semaphore (conceptual trace)
 * ──────────────────────────────────────────── */

void semaphore_init(Semaphore *sem, int value, const char *name) {
    sem->value = value;
    strncpy(sem->name, name, MAX_NAME_LEN - 1);
    sem->name[MAX_NAME_LEN - 1] = '\0';
}

void semaphore_wait(Semaphore *sem) {
    sem->value--;
    printf("  [%s] wait()   -> value = %d%s\n",
           sem->name, sem->value,
           sem->value < 0 ? " (blocked)" : "");
}

void semaphore_signal(Semaphore *sem) {
    sem->value++;
    printf("  [%s] signal() -> value = %d%s\n",
           sem->name, sem->value,
           sem->value <= 0 ? " (woke a process)" : "");
}

/* ────────────────────────────────────────────
 * pthread-based counting semaphore
 * ──────────────────────────────────────────── */

void sem_init_custom(SemaphorePT *sem, int value) {
    sem->value = value;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void sem_destroy_custom(SemaphorePT *sem) {
    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->cond);
}

/* P() — decrement; block while value is zero */
void sem_wait_custom(SemaphorePT *sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value <= 0)
        pthread_cond_wait(&sem->cond, &sem->mutex);
    sem->value--;
    pthread_mutex_unlock(&sem->mutex);
}

/* V() — increment and wake one blocked waiter */
void sem_signal_custom(SemaphorePT *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

/* ────────────────────────────────────────────
 * Conceptual semaphore demo (logical trace)
 * ──────────────────────────────────────────── */

void demo_semaphore(void) {
    print_header("Semaphore Demo (Logical Trace)");
    printf("  A counting semaphore (initial value = 3) models a resource pool.\n");
    printf("  wait()   = acquire resource  (P / down)\n");
    printf("  signal() = release resource  (V / up)\n\n");

    Semaphore s;
    semaphore_init(&s, 3, "tickets");

    printf("  Acquire 3 resources:\n");
    semaphore_wait(&s);
    semaphore_wait(&s);
    semaphore_wait(&s);

    printf("\n  Try to acquire a 4th (would block a real process):\n");
    semaphore_wait(&s);

    printf("\n  Release 2 resources:\n");
    semaphore_signal(&s);
    semaphore_signal(&s);
}
