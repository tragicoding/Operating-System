#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include "common.h"

typedef enum {
    PAGE_REPLACE_FIFO,
    PAGE_REPLACE_LRU,
    PAGE_REPLACE_OPTIMAL,
    PAGE_REPLACE_CLOCK
} PageReplaceAlgo;

typedef struct {
    int  frame_number;
    bool valid;
    bool dirty;
    bool referenced;
} PageTableEntry;

void demo_paging(void);
void demo_page_replacement(PageReplaceAlgo algo);
void demo_tlb(void);
void run_ch08_demo(void);

#endif /* VIRTUAL_MEMORY_H */
