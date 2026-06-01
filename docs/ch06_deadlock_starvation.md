# CH06 — Deadlock and Starvation

## Deadlock Definition

A **deadlock** is a state in which a set of processes are permanently blocked because each process is waiting for a resource held by another process in the set. No process can ever make progress.

## Four Coffman Conditions

All four conditions must hold simultaneously for a deadlock to occur. Breaking any one of them prevents deadlock.

| # | Condition | Description |
|---|-----------|-------------|
| 1 | **Mutual Exclusion** | At least one resource must be held in a non-shareable mode — only one thread can use it at a time. |
| 2 | **Hold and Wait** | A thread holds at least one resource and is waiting to acquire additional resources held by other threads. |
| 3 | **No Preemption** | Resources cannot be forcibly taken from a thread; they must be released voluntarily. |
| 4 | **Circular Wait** | A set {P0, P1, …, Pn} exists where P0 waits for P1, P1 waits for P2, …, Pn waits for P0. |

## Deadlock Prevention

Eliminate at least one of the four conditions at design time:

- **Deny Mutual Exclusion** — make resources shareable where possible (e.g., read-only files).
- **Deny Hold and Wait** — require a process to request all resources at once before starting.
- **Allow Preemption** — if a resource cannot be granted, release all currently held resources.
- **Deny Circular Wait** — impose a total ordering on all resource types; always acquire in order.

**Trade-off:** Prevention is conservative — it often leads to low resource utilisation.

## Deadlock Avoidance

The OS dynamically checks whether granting a request keeps the system in a **safe state** — one where there is at least one execution order in which all processes can finish. If the request would lead to an unsafe state, it is deferred.

Requires advance knowledge of maximum resource needs (`Max` matrix).

## Deadlock Detection

Instead of preventing or avoiding deadlock, let it happen and then detect and recover:

1. Run a detection algorithm periodically (e.g., resource-allocation graph cycle detection).
2. If a deadlock is detected, recover by:
   - **Aborting** one or more processes.
   - **Preempting** resources from selected victims.

## Banker's Algorithm

Dijkstra's Banker's Algorithm (1965) is the classic avoidance algorithm.

### State
- `Available[m]` — how many units of each resource type are free.
- `Max[n][m]` — maximum demand of each process.
- `Allocation[n][m]` — currently allocated to each process.
- `Need[n][m]` = `Max - Allocation` — remaining demand.

### Safety Algorithm

```
work  = Available
finish[i] = false  for all i

repeat:
  find i such that: finish[i] == false  AND  Need[i] <= work
  if found:
    work    += Allocation[i]   // process i can finish and release
    finish[i] = true
until no such i found

if all finish[i] == true  →  SAFE (safe sequence found)
else                       →  UNSAFE (deadlock possible)
```

### Example (5 processes, 3 resource types A B C)

```
         Max         Alloc       Need
P0:  [7  5  3]   [0  1  0]   [7  4  3]
P1:  [3  2  2]   [2  0  0]   [1  2  2]
P2:  [9  0  2]   [3  0  2]   [6  0  0]
P3:  [2  2  2]   [2  1  1]   [0  1  1]
P4:  [4  3  3]   [0  0  2]   [4  3  1]
Available: [3  3  2]
```

Safe sequence: **P1 → P3 → P4 → P0 → P2**

## Resource Allocation Graph (RAG)

A directed graph where:
- **Request edge P → R** means process P is waiting for resource R.
- **Assignment edge R → P** means resource R is allocated to process P.

For resources with a single instance: **a cycle in the RAG ⟺ deadlock**.

### Cycle detection

DFS with a recursion stack (`in_stack[]`): if we reach a node already in the current stack, a back-edge (cycle) exists.

## Dining Philosophers Problem

Five philosophers sit at a round table with one fork between each pair. Each needs both adjacent forks to eat.

### Naive approach — deadlocks

All philosophers pick up their left fork simultaneously → everyone holds one fork, everyone waits for the right fork → circular wait → deadlock.

### Safe approach — resource ordering

Always acquire the lower-numbered fork first:

- P0 needs forks 0 & 1 → picks 0 then 1 (unchanged)
- P4 needs forks 4 & 0 → picks **0 then 4** (reversed!)

P0 and P4 both compete for fork 0 first. One of them must block, breaking the circular chain.

---

## How to Run

```bash
cd /home/jskim/CAU/Operating-System
make && ./os_sim   # select option 6
```

---

## Example Output

```
Deadlock Concept Demo
  Thread-A: acquired Resource-A
  Thread-B: acquired Resource-B
  Thread-A: Resource-B BLOCKED — holds A, needs B  ← CIRCULAR WAIT
  Thread-B: Resource-A BLOCKED — holds B, needs A  ← CIRCULAR WAIT
  Both threads resolved — no permanent hang.

Resource Allocation Graph (RAG)
  Scenario 1: No cycle — system is safe.
  Scenario 2: CYCLE FOUND — deadlock possible!
  Trace: P0 → R0 → P1 → R1 → P0

Banker's Algorithm
  Safety check:
    P1: need=[1 2 2] <= work=[3 3 2] -> work=[5 3 2] ✓
    P3: need=[0 1 1] <= work=[5 3 2] -> work=[7 4 3] ✓
    ...
  SAFE STATE — safe sequence: P1 → P3 → P4 → P0 → P2

Dining Philosophers
  Philosopher-4 eating (forks 4 & 0)  [pick order: 0 then 4]
  All philosophers finished — no deadlock.
```

---

## What This Implementation Demonstrates

1. **The deadlock position can be entered and escaped** — the trylock demo proves both threads reach the circular-wait state simultaneously; breaking any Coffman condition (here: we add "preemption" via trylock + release) resolves it.

2. **RAG cycle detection is straightforward DFS** — the same algorithm that detects back-edges in any directed graph directly detects deadlock in single-instance resource graphs.

3. **Banker's algorithm is conservative** — it finds a safe sequence if one exists; when available resources are exhausted, even a clearly deadlocked state is correctly identified as unsafe.

4. **Resource ordering is the simplest deadlock-prevention technique** — a single rule (always lock lower-numbered resource first) eliminates circular wait with zero runtime overhead.

5. **Starvation vs Deadlock** — in the first-readers-preference readers-writers solution (CH05), writers can starve even without deadlock. Starvation requires **fairness** mechanisms (aging, fair queues), whereas deadlock requires breaking a Coffman condition.
