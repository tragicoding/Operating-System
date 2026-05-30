#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "common.h"

typedef struct {
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int max_need[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];
    int num_processes;
    int num_resources;
} RAG;

bool banker_algorithm(RAG *rag, int safe_sequence[]);
void demo_deadlock_detection(void);
void demo_banker_algorithm(void);
void run_ch06_demo(void);

#endif /* DEADLOCK_H */
