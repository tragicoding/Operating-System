#include "deadlock.h"

/*
 * Resource Allocation Graph (RAG) with cycle detection.
 *
 * Nodes:
 *   IDs 0 .. P-1        : processes  (P0, P1, ...)
 *   IDs P .. P+R-1      : resources  (R0, R1, ...)
 *
 * Directed edges:
 *   Process p → Resource r  (request edge):   node p  → node P+r
 *   Resource r → Process p  (assignment edge): node P+r → node p
 *
 * Cycle in a RAG with single-instance resources ⟺ deadlock.
 */

static int  adj[RAG_MAX_NODES][RAG_MAX_NODES];
static bool visited[RAG_MAX_NODES];
static bool in_stack[RAG_MAX_NODES];

/* DFS-based cycle detection (returns true if a back-edge is found) */
static bool dfs(int u, int total) {
    visited[u]  = true;
    in_stack[u] = true;

    for (int v = 0; v < total; v++) {
        if (!adj[u][v]) continue;
        if (!visited[v]) {
            if (dfs(v, total)) return true;
        } else if (in_stack[v]) {
            return true;  /* back-edge = cycle */
        }
    }

    in_stack[u] = false;
    return false;
}

static bool rag_has_cycle(int num_procs, int num_rsrcs) {
    int total = num_procs + num_rsrcs;
    memset(visited,  0, sizeof(visited));
    memset(in_stack, 0, sizeof(in_stack));

    for (int i = 0; i < total; i++)
        if (!visited[i] && dfs(i, total))
            return true;
    return false;
}

/* Build the adjacency matrix from a RAGraph struct */
static void build_adj(const RAGraph *g) {
    int P = g->num_processes;
    int R = g->num_resources;
    int total = P + R;

    memset(adj, 0, sizeof(adj));
    for (int p = 0; p < P; p++)
        for (int r = 0; r < R; r++) {
            if (g->request[p][r]) adj[p][P + r]     = 1;  /* P→R */
            if (g->assign[r][p])  adj[P + r][p]     = 1;  /* R→P */
        }
    (void)total;
}

/* Pretty-print the graph */
static void print_rag(const RAGraph *g) {
    printf("  Processes: ");
    for (int p = 0; p < g->num_processes; p++)
        printf("P%d%s", p, p < g->num_processes - 1 ? ", " : "\n");

    printf("  Resources: ");
    for (int r = 0; r < g->num_resources; r++)
        printf("R%d%s", r, r < g->num_resources - 1 ? ", " : "\n");

    printf("\n  Request edges (P → R):\n");
    bool any = false;
    for (int p = 0; p < g->num_processes; p++)
        for (int r = 0; r < g->num_resources; r++)
            if (g->request[p][r]) {
                printf("    P%d --> R%d\n", p, r);
                any = true;
            }
    if (!any) printf("    (none)\n");

    printf("\n  Assignment edges (R --> P):\n");
    any = false;
    for (int r = 0; r < g->num_resources; r++)
        for (int p = 0; p < g->num_processes; p++)
            if (g->assign[r][p]) {
                printf("    R%d --> P%d\n", r, p);
                any = true;
            }
    if (!any) printf("    (none)\n");
}

void demo_rag(void) {
    print_header("Resource Allocation Graph (RAG)");

    /* ── Scenario 1: no cycle ── */
    printf("  Scenario 1: No cycle — safe state\n");
    printf("  ─────────────────────────────────\n");

    RAGraph g1 = {
        .num_processes = 3,
        .num_resources = 2,
        .request = {{1,0},{0,1},{0,0}},  /* P0→R0, P1→R1 */
        .assign  = {{0,0,1},{1,0,0}}     /* R0→P2, R1→P0 */
    };
    print_rag(&g1);
    build_adj(&g1);

    bool cycle1 = rag_has_cycle(g1.num_processes, g1.num_resources);
    printf("\n  Cycle detection: %s\n",
           cycle1 ? ANSI_RED "CYCLE FOUND — deadlock possible!" ANSI_RESET
                  : ANSI_GREEN "No cycle — system is safe." ANSI_RESET);

    printf("\n");

    /* ── Scenario 2: cycle present ── */
    printf("  Scenario 2: Cycle present — deadlock\n");
    printf("  ────────────────────────────────────\n");

    /*
     * P0 requests R0  (R0 held by P1)
     * P1 requests R1  (R1 held by P0)
     * Cycle: P0 → R0 → P1 → R1 → P0
     */
    RAGraph g2 = {
        .num_processes = 2,
        .num_resources = 2,
        .request = {{1,0},{0,1}},   /* P0→R0, P1→R1 */
        .assign  = {{0,1},{1,0}}    /* R0→P1, R1→P0 */
    };
    print_rag(&g2);
    printf("\n  Trace: P0 → R0 → P1 → R1 → P0\n");
    build_adj(&g2);

    bool cycle2 = rag_has_cycle(g2.num_processes, g2.num_resources);
    printf("\n  Cycle detection: %s\n",
           cycle2 ? ANSI_RED "CYCLE FOUND — deadlock possible!" ANSI_RESET
                  : ANSI_GREEN "No cycle — system is safe." ANSI_RESET);
}
