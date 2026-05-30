#include "common.h"
#include "process.h"
#include "thread_sim.h"
#include "semaphore_sim.h"
#include "deadlock.h"
#include "memory.h"
#include "virtual_memory.h"

/* Forward declarations for chapter entry points not in headers yet */
void run_ch01_demo(void);
void run_ch02_demo(void);

static void print_main_menu(void) {
    clear_screen();
    printf(ANSI_BOLD ANSI_CYAN);
    printf("============================================================\n");
    printf("        Operating System Concept Simulator (C11)            \n");
    printf("============================================================\n");
    printf(ANSI_RESET);
    printf("\n");
    printf("  " ANSI_BOLD "[1]" ANSI_RESET " CH01 - Computer System Overview\n");
    printf("  " ANSI_BOLD "[2]" ANSI_RESET " CH02 - Operating System Overview\n");
    printf("  " ANSI_BOLD "[3]" ANSI_RESET " CH03 - Process Description and Control\n");
    printf("  " ANSI_BOLD "[4]" ANSI_RESET " CH04 - Threads\n");
    printf("  " ANSI_BOLD "[5]" ANSI_RESET " CH05 - Concurrency and Synchronization\n");
    printf("  " ANSI_BOLD "[6]" ANSI_RESET " CH06 - Deadlock and Starvation\n");
    printf("  " ANSI_BOLD "[7]" ANSI_RESET " CH07 - Memory Management\n");
    printf("  " ANSI_BOLD "[8]" ANSI_RESET " CH08 - Virtual Memory\n");
    printf("  " ANSI_BOLD "[0]" ANSI_RESET " Exit\n");
    printf("\n");
    printf("  Select an option: ");
}

int main(void) {
    int choice;

    while (1) {
        print_main_menu();

        if (scanf("%d", &choice) != 1) {
            /* Flush invalid input */
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            continue;
        }
        /* Consume trailing newline so press_enter_to_continue works */
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        switch (choice) {
            case 1: run_ch01_demo(); break;
            case 2: run_ch02_demo(); break;
            case 3: run_ch03_demo(); break;
            case 4: run_ch04_demo(); break;
            case 5: run_ch05_demo(); break;
            case 6: run_ch06_demo(); break;
            case 7: run_ch07_demo(); break;
            case 8: run_ch08_demo(); break;
            case 0:
                printf(ANSI_GREEN "\n  Goodbye!\n\n" ANSI_RESET);
                return 0;
            default:
                printf(ANSI_RED "\n  Invalid option. Try again.\n" ANSI_RESET);
                press_enter_to_continue();
                break;
        }
    }
}
