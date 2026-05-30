# CH03: Process Description and Control

## Concept Summary

A **process** is a program in execution — the OS's unit of work.  
The OS tracks every process through a **Process Control Block (PCB)**, which
holds everything needed to suspend and later resume execution: the program
counter, CPU registers, memory bounds, and accounting data.

### Process States

```
          admit
  [NEW] ---------> [READY] <-----------+
                     |   ^             |
            dispatch |   | preempt     | I/O done
                     v   |             |
                  [RUNNING] ---> [BLOCKED]
                     |
                exit |
                     v
                  [EXIT]
```

| State   | Meaning |
|---------|---------|
| NEW     | Process created; not yet admitted to the ready queue |
| READY   | Waiting for CPU assignment |
| RUNNING | Currently executing on the CPU |
| BLOCKED | Waiting for I/O or an event |
| EXIT    | Finished; resources being freed |

### Process Control Block (PCB)

| Field | Type | Purpose |
|-------|------|---------|
| `pid` | `int` | Unique process identifier |
| `name` | `char[32]` | Human-readable name |
| `state` | `ProcessState` | Current state in the state machine |
| `priority` | `int` | Scheduling priority (higher = more important) |
| `program_counter` | `int` | Address of the next instruction |
| `registers[4]` | `int[4]` | Saved CPU registers R0–R3 |
| `memory_base` | `int` | Start of address space |
| `memory_limit` | `int` | Size of address space |
| `cpu_time` | `int` | Total CPU ticks consumed |
| `waiting_time` | `int` | Total ticks spent in the READY queue |

### Context Switch

A **context switch** is the mechanism that lets the OS share one CPU
among multiple processes.  The **dispatcher** performs:

1. **Save** — copy CPU registers / PC into the _outgoing_ PCB.
2. **Load** — restore CPU registers / PC from the _incoming_ PCB.
3. **Handover** — set incoming process to RUNNING; resume execution.

Context-switch time is pure overhead — the system does no useful work
during a switch.

---

## Implemented Files

| File | Responsibility |
|------|---------------|
| `include/process.h` | `ProcessState` enum, `PCB` struct, all declarations |
| `include/scheduler.h` | `Dispatcher` struct declaration |
| `src/ch03_process/pcb.c` | `create_process`, `print_pcb`, `change_process_state` |
| `src/ch03_process/process_state_machine.c` | `simulate_process_state_transition` |
| `src/ch03_process/dispatcher.c` | `dispatcher_switch`, `simulate_context_switch`, `run_ch03_process_demo` |

---

## How to Run

```bash
make run       # build and launch the menu
# Select [3] CH03 - Process Description and Control
# Then choose:
#   [1] Process State Machine
#   [2] Dispatcher & Context Switch
#   [3] Inspect PCB Structure
```

---

## Example Output

### Option 1 — Process State Machine

```
── Phase 2  |  Scheduler picks highest-priority process ──
  [PID 1] Process-Alpha   READY    -->  RUNNING

── Phase 3  |  Alpha requests I/O  (RUNNING -> BLOCKED) ──
  [PID 1] Process-Alpha   RUNNING  -->  BLOCKED
  [PID 2] Process-Beta    READY    -->  RUNNING

── Phase 4  |  Alpha's I/O completes  (BLOCKED -> READY) ──
  [PID 1] Process-Alpha   BLOCKED  -->  READY

── Phase 5  |  Beta's time quantum expires  (RUNNING -> READY) ──
  [PID 2] Process-Beta    RUNNING  -->  READY
  [PID 1] Process-Alpha   READY    -->  RUNNING
```

### Option 2 — Dispatcher & Context Switch

```
┌─ Context Switch #2 ──────────────────────────────────
│ [SAVE]  PID=102 Compiler   PC=0x2B18  R0=55  R1=0  R2=42  R3=0
│ [LOAD]  PID=101 Editor     PC=0x1A00  R0=7   R1=14 R2=0   R3=0
└─────────────────────────────────────────────────────
  [PID 101] Editor    READY  -->  RUNNING
```

---

## What This Demonstrates

- How the OS represents a process in memory (PCB).
- The valid state transitions and the events that trigger each one.
- How illegal transitions (e.g. BLOCKED → RUNNING directly) are detected.
- The exact save / restore mechanics of a context switch.
- Why context-switch overhead matters for scheduler design.
