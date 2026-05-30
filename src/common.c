#include "common.h"

void print_header(const char *title) {
    printf("\n" ANSI_BOLD ANSI_CYAN);
    print_separator();
    printf("  %s\n", title);
    print_separator();
    printf(ANSI_RESET);
}

void print_separator(void) {
    printf("============================================================\n");
}

void press_enter_to_continue(void) {
    printf("\n" ANSI_YELLOW "Press Enter to continue..." ANSI_RESET);
    while (getchar() != '\n')
        ;
}

void clear_screen(void) {
    printf("\033[2J\033[H");
}
