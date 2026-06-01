#include "deadlock.h"

/*
 * Banker's Algorithm — Deadlock Avoidance (E.W. Dijkstra, 1965)
 *
 * The OS acts as a banker: it only grants a resource request if the resulting
 * state is still "safe" — i.e., there exists at least one execution order
 * (safe sequence) in which every process can finish.
 *
 * Safety algorithm:
 *   1. work = available
 *   2. Find an unfinished process whose need ≤ work
 *   3. Pretend it finishes: work += allocation[i]; mark finished
 *   4. Repeat until no more processes can finish
 *   5. If all finished → SAFE; otherwise → UNSAFE
 */

void banker_compute_need(BankState *s) {
    for (int p = 0; p < s->num_processes; p++)
        for (int r = 0; r < s->num_resources; r++)
            s->need[p][r] = s->max_need[p][r] - s->allocation[p][r];
}

bool banker_safe(BankState *s, int seq[]) {
    int  work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES];

    for (int r = 0; r < s->num_resources; r++)
        work[r] = s->available[r];
    for (int p = 0; p < s->num_processes; p++)
        finish[p] = false;

    int seq_idx = 0, found;
    do {
        found = 0;
        for (int p = 0; p < s->num_processes; p++) {
            if (finish[p]) continue;

            bool can = true;
            for (int r = 0; r < s->num_resources; r++)
                if (s->need[p][r] > work[r]) { can = false; break; }

            if (can) {
                printf("    P%d: need=[", p);
                for (int r = 0; r < s->num_resources; r++)
                    printf("%d%s", s->need[p][r], r < s->num_resources-1 ? " " : "");
                printf("] <= work=[");
                for (int r = 0; r < s->num_resources; r++)
                    printf("%d%s", work[r], r < s->num_resources-1 ? " " : "");

                for (int r = 0; r < s->num_resources; r++)
                    work[r] += s->allocation[p][r];

                printf("] -> work=[");
                for (int r = 0; r < s->num_resources; r++)
                    printf("%d%s", work[r], r < s->num_resources-1 ? " " : "");
                printf("] " ANSI_GREEN "✓" ANSI_RESET "\n");

                finish[p]      = true;
                seq[seq_idx++] = p;
                found          = 1;
            }
        }
    } while (found);

    for (int p = 0; p < s->num_processes; p++)
        if (!finish[p]) return false;
    return true;
}

/* Print the full state table */
static void print_state(const BankState *s, const char *label) {
    int P = s->num_processes, R = s->num_resources;
    printf("  %s\n", label);
    printf("  %-4s  %-*s  %-*s  %-*s\n",
           "Proc",
           R * 3, "Max",
           R * 3, "Allocation",
           R * 3, "Need");
    for (int p = 0; p < P; p++) {
        printf("  P%-3d  [", p);
        for (int r = 0; r < R; r++) printf("%d%s", s->max_need[p][r],   r<R-1?" ":"]  [");
        for (int r = 0; r < R; r++) printf("%d%s", s->allocation[p][r], r<R-1?" ":"]  [");
        for (int r = 0; r < R; r++) printf("%d%s", s->need[p][r],       r<R-1?" ":"]");
        printf("\n");
    }
    printf("  Available: [");
    for (int r = 0; r < R; r++) printf("%d%s", s->available[r], r<R-1?" ":"]");
    printf("\n\n");
}

void demo_bankers_algorithm(void) {
    print_header("Banker's Algorithm (Deadlock Avoidance)");

    /* ── Scenario 1: safe state (classic 5-process, 3-resource example) ── */
    printf("  Scenario 1: Safe State\n");
    printf("  ──────────────────────\n");

    BankState s1 = {
        .num_processes = 5,
        .num_resources = 3,
        .available  = {3, 3, 2},
        .max_need   = {{7,5,3},{3,2,2},{9,0,2},{2,2,2},{4,3,3}},
        .allocation = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}}
    };
    banker_compute_need(&s1);
    print_state(&s1, "Initial state (A, B, C):");

    int seq1[MAX_PROCESSES] = {0};
    printf("  Safety check:\n");
    bool safe1 = banker_safe(&s1, seq1);

    if (safe1) {
        printf("\n  " ANSI_GREEN "SAFE STATE" ANSI_RESET " — safe sequence: ");
        for (int i = 0; i < s1.num_processes; i++)
            printf("P%d%s", seq1[i], i < s1.num_processes - 1 ? " → " : "\n");
    } else {
        printf("\n  " ANSI_RED "UNSAFE STATE" ANSI_RESET "\n");
    }

    printf("\n");

    /* ── Scenario 2: unsafe state ── */
    printf("  Scenario 2: Unsafe State\n");
    printf("  ────────────────────────\n");

    /*
     * All resources are fully allocated; available = [0,0,0].
     * No process can satisfy its remaining need → no safe sequence.
     */
    BankState s2 = {
        .num_processes = 3,
        .num_resources = 2,
        .available  = {0, 0},
        .max_need   = {{3,2},{2,3},{2,2}},
        .allocation = {{1,1},{1,1},{1,1}}
    };
    banker_compute_need(&s2);
    print_state(&s2, "Initial state (A, B):");

    int seq2[MAX_PROCESSES] = {0};
    printf("  Safety check:\n");
    bool safe2 = banker_safe(&s2, seq2);

    if (safe2) {
        printf("\n  " ANSI_GREEN "SAFE STATE" ANSI_RESET "\n");
    } else {
        printf("\n  " ANSI_RED "UNSAFE STATE" ANSI_RESET
               " — no process can proceed with available=[0,0].\n"
               "  System is deadlocked.\n");
    }
}
