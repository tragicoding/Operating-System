#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#define MAX_NAME_LEN  64
#define MAX_PROCESSES 32
#define MAX_THREADS   64
#define MAX_RESOURCES 16

#define ANSI_RESET  "\033[0m"
#define ANSI_BOLD   "\033[1m"
#define ANSI_RED    "\033[31m"
#define ANSI_GREEN  "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_CYAN   "\033[36m"

void print_header(const char *title);
void print_separator(void);
void press_enter_to_continue(void);
void clear_screen(void);

#endif /* COMMON_H */
