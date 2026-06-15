# 07 Thread Scaling

## Goal

Show scaling limits and oversubscription by running the same workload with increasing thread counts.

Example command:

```sh
./build/benchmarks/thread_scaling --max-threads 32 --output results/thread_scaling.csv
```

## Hypothesis

Performance should improve while additional threads map well to available cores, then flatten or degrade when the workload becomes limited by memory bandwidth, synchronization, or scheduler overhead.

## Thread Counts

Test at least:

- 1
- 2
- 4
- 8
- 16

If your machine supports more, continue up to `--max-threads`.

## Workload Choice

Pick a workload that is easy to reason about. Good choices:

- embarrassingly parallel arithmetic over a large array
- independent checksum chunks
- repeated integer math with no shared state

Avoid starting with a workload that secretly measures locks, allocation, or I/O. This benchmark should isolate thread scaling first.

## C++ Concepts To Practice

- dividing work into chunks
- `std::thread::hardware_concurrency`
- joining worker threads
- per-thread partial results
- reducing partial results after joins

## Things To Watch

- Does each thread get roughly equal work?
- Does thread creation time dominate?
- Is the input large enough?
- Are you measuring memory bandwidth instead of CPU work?
- Does oversubscription hurt after a certain point?

## Explanation Prompts

- What thread count was best?
- What happened after physical core count?
- What happened after logical CPU count?
- Did results suggest CPU-bound or memory-bound behavior?

## Checkpoint

This benchmark is done when it runs a configurable range of thread counts and produces a scaling table in CSV.
