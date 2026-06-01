#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "common.h"
#include <pthread.h>
#include <stdatomic.h>

/* ── Banker's Algorithm state ── */
typedef struct {
    int available[MAX_RESOURCES];
    int max_need[MAX_PROCESSES][MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];   /* computed: max - alloc */
    int num_processes;
    int num_resources;
} BankState;

/* ── Resource Allocation Graph ── */
#define RAG_MAX_NODES (MAX_PROCESSES + MAX_RESOURCES)

typedef struct {
    int num_processes;
    int num_resources;
    /* request[p][r]=1 : process p requests resource r  (P→R edge) */
    int request[MAX_PROCESSES][MAX_RESOURCES];
    /* assign[r][p]=1  : resource r is assigned to process p (R→P edge) */
    int assign[MAX_RESOURCES][MAX_PROCESSES];
} RAGraph;

/* ── Function declarations ── */
void demo_deadlock_concept(void);
void demo_rag(void);

void banker_compute_need(BankState *s);
bool banker_safe(BankState *s, int seq[]);
void demo_bankers_algorithm(void);

void demo_dining_philosophers(void);

void run_ch06_deadlock_demo(void);
void run_ch06_demo(void);

#endif /* DEADLOCK_H */
