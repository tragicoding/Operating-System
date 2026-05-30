#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

typedef enum {
    ALLOC_FIRST_FIT,
    ALLOC_BEST_FIT,
    ALLOC_WORST_FIT
} AllocStrategy;

typedef struct {
    int  start;
    int  size;
    bool is_free;
    int  pid;
} MemoryBlock;

void demo_fixed_partitioning(void);
void demo_dynamic_partitioning(AllocStrategy strategy);
void demo_fragmentation(void);
void run_ch07_demo(void);

#endif /* MEMORY_H */
