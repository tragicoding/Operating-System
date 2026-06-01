#include "semaphore_sim.h"

/*
 * Readers-Writers Problem (first readers-preference variant).
 *
 * Rules:
 *   - Any number of readers may read simultaneously.
 *   - A writer needs exclusive access (no readers, no other writers).
 *
 * Shared state:
 *   reader_count — tracks how many readers are currently inside.
 *   rw_mutex     — locks the data; first reader acquires it, last releases it.
 *   count_mutex  — protects reader_count itself.
 */

#define N_READERS 3
#define N_WRITERS 2
#define ITERS     3

static int reader_count = 0;
static int shared_data  = 0;

static pthread_mutex_t rw_mutex    = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t print_lock  = PTHREAD_MUTEX_INITIALIZER;

static void *reader(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITERS; i++) {
        /* ── Entry: first reader blocks writers ── */
        pthread_mutex_lock(&count_mutex);
        reader_count++;
        if (reader_count == 1)
            pthread_mutex_lock(&rw_mutex);  /* first reader locks out writers */
        pthread_mutex_unlock(&count_mutex);

        /* ── Read ── */
        pthread_mutex_lock(&print_lock);
        printf("  Reader-%d reading  data=%d  (active readers: %d)\n",
               id, shared_data, reader_count);
        pthread_mutex_unlock(&print_lock);

        /* ── Exit: last reader releases writers ── */
        pthread_mutex_lock(&count_mutex);
        reader_count--;
        if (reader_count == 0)
            pthread_mutex_unlock(&rw_mutex);  /* last reader lets writers in */
        pthread_mutex_unlock(&count_mutex);
    }
    return NULL;
}

static void *writer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITERS; i++) {
        pthread_mutex_lock(&rw_mutex);  /* exclusive lock */

        shared_data++;
        pthread_mutex_lock(&print_lock);
        printf("  Writer-%d wrote    data=%d\n", id, shared_data);
        pthread_mutex_unlock(&print_lock);

        pthread_mutex_unlock(&rw_mutex);
    }
    return NULL;
}

void demo_readers_writers(void) {
    print_header("Readers-Writers Problem");
    printf("  Readers: %d | Writers: %d | Iterations: %d\n",
           N_READERS, N_WRITERS, ITERS);
    printf("  Readers share access; writers hold exclusive lock.\n\n");

    reader_count = 0;
    shared_data  = 0;

    pthread_t readers[N_READERS], writers[N_WRITERS];
    int       rids[N_READERS],    wids[N_WRITERS];

    for (int i = 0; i < N_READERS; i++) {
        rids[i] = i;
        pthread_create(&readers[i], NULL, reader, &rids[i]);
    }
    for (int i = 0; i < N_WRITERS; i++) {
        wids[i] = i;
        pthread_create(&writers[i], NULL, writer, &wids[i]);
    }
    for (int i = 0; i < N_READERS; i++) pthread_join(readers[i], NULL);
    for (int i = 0; i < N_WRITERS; i++) pthread_join(writers[i], NULL);

    printf("\n  Final shared_data: " ANSI_GREEN "%d" ANSI_RESET
           "  (expected %d writers × %d iters = %d)\n",
           shared_data, N_WRITERS, ITERS, N_WRITERS * ITERS);
}
