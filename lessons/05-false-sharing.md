# 05 False Sharing

## Goal

Demonstrate cache line contention between threads by comparing adjacent counters with padded counters.

Example command:

```sh
./build/benchmarks/false_sharing --threads 8 --iterations 10 --output results/false_sharing.csv
```

## Hypothesis

Multiple threads incrementing different counters on the same cache line should be slower than incrementing counters separated onto different cache lines.

## Variants

Implement at least:

- `adjacent_counters`
- `padded_counters`

Optional:

- `single_thread`
- `atomic_adjacent`
- `atomic_padded`

## Implementation Hints

Create one counter per thread.

For the adjacent version, store counters next to each other in a vector or array.

For the padded version, create a struct with alignment/padding so each counter is isolated:

- look up `alignas`
- learn the likely cache line size, commonly 64 bytes
- document that cache line size varies by machine

Make sure each thread only updates its own counter. The point is not logical sharing. The point is hardware-level cache line sharing.

## C++ Concepts To Practice

- `std::thread`
- joining threads
- `alignas`
- structs and object layout
- atomics if you choose atomic variants
- thinking about memory layout

## Pitfalls

- Forgetting to join all threads.
- Measuring thread creation more than counter updates.
- Letting the compiler optimize away increments.
- Accidentally using one shared counter instead of one counter per thread.

## Explanation Prompts

- How much faster was the padded version?
- Did the difference grow with more threads?
- Why can independent variables still fight each other?
- What does this teach about structs, arrays, and hot fields?

## Checkpoint

This benchmark is done when adjacent and padded counters are compared across configurable thread counts and the output includes stable CSV rows.
