#ifndef SEMAPHORE_SIM_H
#define SEMAPHORE_SIM_H

#include "common.h"

typedef struct {
    int  value;
    char name[MAX_NAME_LEN];
} Semaphore;

void semaphore_init(Semaphore *sem, int value, const char *name);
void semaphore_wait(Semaphore *sem);
void semaphore_signal(Semaphore *sem);

void demo_mutex(void);
void demo_producer_consumer(void);
void demo_readers_writers(void);
void run_ch05_demo(void);

#endif /* SEMAPHORE_SIM_H */
