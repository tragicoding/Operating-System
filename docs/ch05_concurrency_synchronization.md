# CH05 — Concurrency: Mutual Exclusion and Synchronization

## Concepts

### Race Condition
A race condition occurs when two or more threads access shared data concurrently and the final result depends on the execution order. The classic example is the read-modify-write sequence on a counter: one thread reads the value, another reads the same stale value, both increment, and one write is silently lost.

### Critical Section
A **critical section** is a code region that accesses shared resources and must not execute in more than one thread simultaneously. Three requirements must hold:
- **Mutual exclusion** — only one thread in the CS at a time.
- **Progress** — if no thread is in the CS, one waiting thread must eventually enter.
- **Bounded waiting** — no thread waits forever to enter its CS.

### Mutual Exclusion
Mutual exclusion is the guarantee that only one thread executes a critical section at any given moment. It is the cornerstone of safe concurrent programming.

### Peterson's Algorithm
A classic **software-only** solution for two-process mutual exclusion (Gary Peterson, 1981).

```c
flag[i] = 1;          // "I want to enter"
turn    = j;          // "But you go first"
while (flag[j] && turn == j)
    ;                 // busy-wait
// --- critical section ---
flag[i] = 0;          // "I'm done"
```

`flag[i]` expresses intent; `turn` resolves simultaneous attempts. On modern CPUs, `atomic_int` (C11 `<stdatomic.h>`) is required to prevent compiler/CPU reordering.

**Limitation:** busy-waiting burns CPU. Modern OS solutions use blocking primitives (semaphores, mutexes).

### Semaphore
A semaphore is an integer variable with two atomic operations:
- **wait / P / down** — decrement; block if value would go below zero.
- **signal / V / up** — increment; wake one blocked thread.

| Type | Initial value | Use case |
|------|--------------|----------|
| Binary (mutex) | 1 | mutual exclusion |
| Counting | N | resource pool of size N |

The `SemaphorePT` struct in this project wraps `pthread_mutex_t` + `pthread_cond_t` to implement true blocking.

### Producer-Consumer (Bounded Buffer)
Two thread types share a fixed-size circular buffer.
- **Producer** deposits items; blocks when buffer is full.
- **Consumer** removes items; blocks when buffer is empty.

Three semaphores solve this:
```
sem_empty = BUF_SIZE   // free slots
sem_full  = 0          // filled slots
sem_mutex = 1          // mutual exclusion on buffer operations
```

Producer protocol: `wait(empty) → wait(mutex) → deposit → signal(mutex) → signal(full)`
Consumer protocol: `wait(full)  → wait(mutex) → remove  → signal(mutex) → signal(empty)`

### Readers-Writers Problem
A shared resource (e.g., a database) can be read by many readers simultaneously, but a writer needs exclusive access.

First readers-preference solution:
- A counter `reader_count` tracks active readers.
- The first reader acquires `rw_mutex`, blocking writers.
- Subsequent readers enter freely.
- The last reader releases `rw_mutex`, allowing the next writer.

**Trade-off:** writers may starve if readers arrive continuously (writers-preference or fair variants address this).

---

## How to Run

```bash
cd /home/jskim/CAU/Operating-System
make
./os_sim    # select option 5 from the menu
```

Or build and run in one step:

```bash
make run
```

---

## Example Output

```
==============================
 Race Condition Demo (No Mutex)
==============================
  2 threads each increment shared_counter 100000 times.
  Expected: 200000

  Result WITHOUT mutex: 147382  <-- DATA RACE! lost updates

==============================
 Mutex Solution
==============================
  Result WITH mutex: 200000  (expected 200000) <-- correct!

==============================
 Peterson's Algorithm
==============================
  Thread-0 [CS entry] counter = 0
  Thread-0 [CS exit ] counter = 1
  Thread-1 [CS entry] counter = 1
  Thread-1 [CS exit ] counter = 2
  ...
  Final counter (expected 10): 10  <-- mutual exclusion held!

==============================
 Semaphore Demo (Logical Trace)
==============================
  [tickets] wait()   -> value = 2
  [tickets] wait()   -> value = 1
  [tickets] wait()   -> value = 0
  [tickets] wait()   -> value = -1 (blocked)
  [tickets] signal() -> value = 0 (woke a process)
  [tickets] signal() -> value = 1

==============================
 Producer-Consumer Problem
==============================
  Producer-0 produced item   0  (buf_in=1)
  Consumer-0 consumed item   0  (buf_out=1)
  ...
  All 12 items produced and consumed.

==============================
 Readers-Writers Problem
==============================
  Reader-0 reading  data=0  (active readers: 2)
  Reader-1 reading  data=0  (active readers: 2)
  Writer-0 wrote    data=1
  ...
  Final shared_data: 6  (expected 2 writers x 3 iters = 6)
```

---

## What I Learned

1. **Race conditions are non-deterministic** — the buggy counter demo may sometimes produce the correct answer by luck, making races notoriously hard to reproduce in testing.

2. **Atomic read-modify-write is the fundamental requirement** — a plain `counter++` in C compiles to three machine instructions (load, add, store) that can be interleaved by the scheduler.

3. **Peterson's algorithm is elegant but impractical today** — it needs no OS support, but busy-waiting wastes CPU, and the correct C11 implementation requires `_Atomic` variables to prevent compiler/hardware reordering.

4. **Semaphores unify the mutex and condition-variable patterns** — a binary semaphore is a mutex; a counting semaphore is a resource counter; both are built from the same two operations (P/V).

5. **Semaphore ordering prevents deadlock** — in producer-consumer, acquiring `sem_mutex` *inside* `sem_empty`/`sem_full` (not outside) is critical. Reversing the order causes deadlock.

6. **Readers-writers trades fairness for throughput** — the first-readers variant maximises read concurrency but can starve writers under heavy read load.
