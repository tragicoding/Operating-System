#include "semaphore_sim.h"

void semaphore_init(Semaphore *sem, int value, const char *name) {
    sem->value = value;
    strncpy(sem->name, name, MAX_NAME_LEN - 1);
    sem->name[MAX_NAME_LEN - 1] = '\0';
}

void semaphore_wait(Semaphore *sem) {
    sem->value--;
    printf("  [%s] wait()  -> value = %d%s\n",
           sem->name, sem->value,
           sem->value < 0 ? " (blocked)" : "");
}

void semaphore_signal(Semaphore *sem) {
    sem->value++;
    printf("  [%s] signal() -> value = %d%s\n",
           sem->name, sem->value,
           sem->value <= 0 ? " (woke a process)" : "");
}

void demo_mutex(void) {
    print_header("Mutex Semaphore Demo");
    Semaphore mutex;
    semaphore_init(&mutex, 1, "mutex");
    printf("  Process A entering critical section:\n");
    semaphore_wait(&mutex);
    printf("  Process A inside critical section...\n");
    printf("  Process A leaving critical section:\n");
    semaphore_signal(&mutex);
}

void demo_producer_consumer(void) {
    print_header("Producer-Consumer Problem");
    printf("  [Full simulation coming soon]\n");
}

void demo_readers_writers(void) {
    print_header("Readers-Writers Problem");
    printf("  [Full simulation coming soon]\n");
}

void run_ch05_demo(void) {
    print_header("CH05: Concurrency and Synchronization");
    printf("  Topics covered:\n");
    printf("  - Race conditions and critical sections\n");
    printf("  - Semaphores (binary and counting)\n");
    printf("  - Classic problems: Producer-Consumer, Readers-Writers, Dining Philosophers\n");
    printf("  - Monitors and condition variables\n");
    printf("\n  [Demo not yet implemented — coming soon]\n");
    press_enter_to_continue();
}
