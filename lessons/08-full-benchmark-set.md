# 08 Full Benchmark Set

## Goal

After the MVP, add enough benchmarks to reach the full project target of 6-8 benchmarks.

Recommended order:

1. `allocation`
2. `mutex_queue`
3. `file_io`
4. `mmap_scan`

## Allocation

Must demonstrate allocation overhead and object reuse.

Minimum comparisons:

- repeated allocation
- object reuse
- `std::vector::reserve`
- copy vs move

Learning focus:

- object lifetime
- RAII
- move constructors and move assignment
- capacity vs size
- allocator overhead

## Mutex Queue

Must demonstrate lock contention and producer/consumer behavior.

Minimum implementation:

- bounded queue
- producer threads
- consumer threads
- mutex and condition variables

Learning focus:

- `std::mutex`
- `std::condition_variable`
- waiting in a loop
- shutdown signaling
- throughput under contention

## File I/O

Must demonstrate buffered I/O and block size effects.

Minimum comparisons:

- read/write a large file
- vary block sizes
- report throughput

Learning focus:

- binary file I/O
- block sizes
- OS page cache
- throughput calculations
- separating setup from measured work

## mmap Scan

Must compare read loop vs `mmap`.

Minimum comparisons:

- scan a large file with normal reads
- scan the same file with `mmap`
- compare throughput and caveats

Learning focus:

- POSIX APIs
- virtual memory
- page faults
- page cache
- cleanup with `munmap`

## Checkpoint

This lesson is done when the project has at least 6 benchmark executables with consistent CLI and CSV output.
