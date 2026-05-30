# Operating System Concept Simulator

An educational **Operating System concept simulator** written in C11.  
This is **not** a real kernel — it is a menu-driven CLI tool that demonstrates and visualizes core OS concepts through simulation.

## Chapters

| # | Chapter | Status |
|---|---------|--------|
| 01 | Computer System Overview | Placeholder |
| 02 | Operating System Overview | Placeholder |
| 03 | Process Description and Control | Placeholder |
| 04 | Threads | Placeholder |
| 05 | Concurrency and Synchronization | Placeholder |
| 06 | Deadlock and Starvation | Placeholder |
| 07 | Memory Management | Placeholder |
| 08 | Virtual Memory | Placeholder |

## Build & Run

### Requirements

- GCC (C11)
- GNU Make
- Linux / WSL Ubuntu

### Commands

```bash
make          # build
make run      # build and run
make debug    # build with AddressSanitizer + UBSan
make clean    # remove build artifacts
```

## Project Structure

```
Operating-System/
├── Makefile
├── include/          # Public header files
├── src/
│   ├── main.c        # Main menu
│   ├── common.c      # Shared utilities
│   ├── ch01_computer_system/
│   ├── ch02_os_overview/
│   ├── ch03_process/
│   ├── ch04_threads/
│   ├── ch05_concurrency/
│   ├── ch06_deadlock/
│   ├── ch07_memory_management/
│   └── ch08_virtual_memory/
├── docs/             # Chapter documentation
├── examples/         # Standalone example programs
└── tests/            # Unit tests
```

## License

MIT
