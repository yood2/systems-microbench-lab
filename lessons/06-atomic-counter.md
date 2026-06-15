# 06 Atomic Counter

## Goal

Compare synchronization strategies for incrementing counters.

Minimum variants:

- mutex counter
- atomic counter
- relaxed atomic counter
- sharded counter

Example command:

```sh
./build/benchmarks/atomic_counter --threads 8 --iterations 10 --output results/atomic_counter.csv
```

## Hypothesis

A single shared synchronized counter should scale poorly as threads increase. A sharded counter should reduce contention and usually perform better.

## Variant Notes

`mutex_counter`: protect a normal integer with `std::mutex`.

`atomic_counter`: use `std::atomic` with default memory ordering.

`relaxed_atomic`: use `memory_order_relaxed` when you only need atomicity, not ordering with other data.

`sharded_counter`: each thread increments its own counter, then the benchmark sums all counters at the end.

## C++ Concepts To Practice

- `std::mutex`
- `std::lock_guard`
- `std::atomic`
- memory ordering basics
- per-thread data
- reducing contention

## Important Learning Point

`memory_order_relaxed` does not mean "unsafe increment." It means the increment is atomic, but it does not create ordering guarantees for other memory. That is enough for a standalone counter benchmark.

## Result Interpretation

Compare:

- duration as threads increase
- operations per second
- final counter correctness
- shape of scaling curve

A benchmark is suspicious if the final count is wrong or if all variants have identical timings.

## Checkpoint

This benchmark is done when all four variants produce the expected final count and write comparable CSV rows.
