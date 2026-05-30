#include "memory.h"

void demo_fixed_partitioning(void) {
    print_header("Fixed Partitioning");
    printf("  Memory divided into fixed-size partitions\n");
    printf("  Simple but causes internal fragmentation\n");
    printf("\n  [Full simulation coming soon]\n");
}

void demo_dynamic_partitioning(AllocStrategy strategy) {
    print_header("Dynamic Partitioning");
    const char *names[] = { "First Fit", "Best Fit", "Worst Fit" };
    printf("  Strategy: %s\n", names[strategy]);
    printf("  Reduces internal fragmentation\n");
    printf("  May cause external fragmentation\n");
    printf("\n  [Full simulation coming soon]\n");
}

void demo_fragmentation(void) {
    print_header("Fragmentation Demo");
    printf("  Internal fragmentation: wasted space inside allocated block\n");
    printf("  External fragmentation: free space scattered across memory\n");
    printf("\n  [Full simulation coming soon]\n");
}

void run_ch07_demo(void) {
    print_header("CH07: Memory Management");
    printf("  Topics covered:\n");
    printf("  - Memory hierarchy and allocation\n");
    printf("  - Fixed and dynamic partitioning\n");
    printf("  - Segmentation\n");
    printf("  - Internal and external fragmentation\n");
    printf("\n  [Demo not yet implemented — coming soon]\n");
    press_enter_to_continue();
}
