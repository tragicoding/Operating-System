#include "semaphore_sim.h"

/*
 * Classic Bounded-Buffer Producer-Consumer.
 *
 * Semaphores used:
 *   sem_empty  — counts free slots  (initial = BUF_SIZE)
 *   sem_full   — counts filled slots (initial = 0)
 *   sem_mutex  — binary semaphore protecting the buffer (initial = 1)
 *
 * Producer: wait(empty) → wait(mutex) → deposit → signal(mutex) → signal(full)
 * Consumer: wait(full)  → wait(mutex) → remove  → signal(mutex) → signal(empty)
 */

#define BUF_SIZE    5
#define N_PRODUCERS 2
#define N_CONSUMERS 2
#define ITEMS_EACH  6   /* each producer produces this many items */

static int buffer[BUF_SIZE];
static int buf_in  = 0;
static int buf_out = 0;

static SemaphorePT sem_empty;
static SemaphorePT sem_full;
static SemaphorePT sem_mutex;

static pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

static void *producer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITEMS_EACH; i++) {
        int item = id * 100 + i;

        sem_wait_custom(&sem_empty);        /* wait for a free slot    */
        sem_wait_custom(&sem_mutex);        /* enter critical section  */

        buffer[buf_in] = item;
        buf_in = (buf_in + 1) % BUF_SIZE;

        sem_signal_custom(&sem_mutex);      /* leave critical section  */
        sem_signal_custom(&sem_full);       /* announce new item       */

        pthread_mutex_lock(&print_lock);
        printf("  Producer-%d produced item %3d  (buf_in=%d)\n", id, item, buf_in);
        pthread_mutex_unlock(&print_lock);
    }
    return NULL;
}

static void *consumer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITEMS_EACH; i++) {
        sem_wait_custom(&sem_full);         /* wait for an item        */
        sem_wait_custom(&sem_mutex);        /* enter critical section  */

        int item = buffer[buf_out];
        buf_out = (buf_out + 1) % BUF_SIZE;

        sem_signal_custom(&sem_mutex);      /* leave critical section  */
        sem_signal_custom(&sem_empty);      /* announce freed slot     */

        pthread_mutex_lock(&print_lock);
        printf("  Consumer-%d consumed item %3d  (buf_out=%d)\n", id, item, buf_out);
        pthread_mutex_unlock(&print_lock);
    }
    return NULL;
}

void demo_producer_consumer(void) {
    print_header("Producer-Consumer Problem");
    printf("  Buffer: %d slots | Producers: %d | Consumers: %d | Items each: %d\n",
           BUF_SIZE, N_PRODUCERS, N_CONSUMERS, ITEMS_EACH);
    printf("  Total items in flight: %d\n\n", N_PRODUCERS * ITEMS_EACH);

    sem_init_custom(&sem_empty, BUF_SIZE);
    sem_init_custom(&sem_full,  0);
    sem_init_custom(&sem_mutex, 1);

    buf_in = buf_out = 0;

    pthread_t prod[N_PRODUCERS], cons[N_CONSUMERS];
    int       pids[N_PRODUCERS],  cids[N_CONSUMERS];

    for (int i = 0; i < N_PRODUCERS; i++) {
        pids[i] = i;
        pthread_create(&prod[i], NULL, producer, &pids[i]);
    }
    for (int i = 0; i < N_CONSUMERS; i++) {
        cids[i] = i;
        pthread_create(&cons[i], NULL, consumer, &cids[i]);
    }
    for (int i = 0; i < N_PRODUCERS; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < N_CONSUMERS; i++) pthread_join(cons[i], NULL);

    sem_destroy_custom(&sem_empty);
    sem_destroy_custom(&sem_full);
    sem_destroy_custom(&sem_mutex);

    printf("\n  " ANSI_GREEN "All %d items produced and consumed." ANSI_RESET "\n",
           N_PRODUCERS * ITEMS_EACH);
}
