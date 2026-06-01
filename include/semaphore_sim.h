#ifndef SEMAPHORE_SIM_H
#define SEMAPHORE_SIM_H

#include "common.h"
#include <pthread.h>

/* ── Simple logical semaphore (conceptual, no real blocking) ── */
typedef struct {
    int  value;
    char name[MAX_NAME_LEN];
} Semaphore;

void semaphore_init(Semaphore *sem, int value, const char *name);
void semaphore_wait(Semaphore *sem);
void semaphore_signal(Semaphore *sem);

/* ── pthread-based counting semaphore ── */
typedef struct {
    int             value;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} SemaphorePT;

void sem_init_custom(SemaphorePT *sem, int value);
void sem_destroy_custom(SemaphorePT *sem);
void sem_wait_custom(SemaphorePT *sem);    /* P() / down() */
void sem_signal_custom(SemaphorePT *sem);  /* V() / up()   */

/* ── Demo entry points ── */
void demo_race_condition(void);
void demo_mutex_solution(void);
void demo_peterson(void);
void demo_semaphore(void);
void demo_producer_consumer(void);
void demo_readers_writers(void);

void run_ch05_concurrency_demo(void);
void run_ch05_demo(void);

#endif /* SEMAPHORE_SIM_H */
