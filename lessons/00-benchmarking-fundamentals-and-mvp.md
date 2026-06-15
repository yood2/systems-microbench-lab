# Benchmarking Fundamentals and the MVP Benchmarks

## What Benchmark Software Does

Benchmark software is controlled measurement software. It runs a specific piece
of code under specific conditions and records how long it took or how much work
it completed.

A benchmark usually has five parts:

1. Setup: create inputs, allocate memory, open files, create worker threads, or
   prepare any state needed by the test.
2. Timed region: run only the operation you want to measure.
3. Guard against fake work: make sure the compiler cannot delete the work
   because the result is unused.
4. Reporting: write duration, throughput, input size, thread count, and notes.
5. Repetition: run more than once because one measurement can be noisy.

The job of a benchmark is not just to print a number. The job is to answer a
question.

Example question:

> Does reading memory sequentially run faster than reading the same memory in a
> large-stride pattern?

The code should be designed so the answer mostly reflects that question, not a
random side effect.

## Why Microbenchmarks Are Tricky

Microbenchmarks are small, which is good for learning, but that also makes them
easy to fool.

Common problems:

- The compiler removes the measured work.
- The benchmark accidentally measures setup instead of the operation.
- The input is too small, so timing noise dominates.
- Debug builds produce meaningless performance numbers.
- CPU frequency changes during the run.
- Other processes interrupt the benchmark.
- File I/O benchmarks measure the OS page cache instead of the disk.
- Thread benchmarks measure thread creation instead of the work.

This is why every benchmark in this project should include:

- hypothesis
- exact command
- result
- explanation
- caveats

## Basic Metrics

`duration_ms` means elapsed wall-clock time in milliseconds.

`throughput_ops_sec` means operations per second. You define what one operation
means for each benchmark.

For `cache_stride`, one operation can be one array load.

For `atomic_counter`, one operation can be one counter increment.

For file I/O, throughput is often bytes per second instead of operations per
second. The project CSV column is named `throughput_ops_sec`, but your notes can
explain when the unit is really bytes/sec.

## The Four MVP Benchmarks

The MVP benchmarks are chosen because each one teaches a different performance
idea.

### 1. cache_stride

Question:

> How does memory access pattern affect speed?

What it measures:

- one thread
- one large array
- different access strides
- cache locality
- hardware prefetch behavior

Core idea:

Modern CPUs do not fetch single bytes from memory one at a time. They move data
in cache lines. A common cache line size is 64 bytes, though you should treat
that as machine-dependent. If your program reads memory sequentially, the CPU can
often predict what comes next and fetch it early. If your program jumps around,
it may waste cache lines and miss more often.

Expected shape:

- small strides should usually be faster
- large strides should usually be slower
- results become more interesting when the array is larger than CPU cache

This benchmark is mostly about memory locality, not threading.

### 2. false_sharing

Question:

> Can independent thread-local counters still slow each other down?

What it measures:

- multiple threads
- one counter per thread
- adjacent counters vs padded counters
- cache coherence traffic

Core idea:

False sharing happens when different threads update different variables that
happen to live on the same cache line. The variables are logically independent,
but the hardware tracks ownership at cache-line granularity. If two cores keep
writing to the same cache line, that line bounces between cores.

Expected shape:

- adjacent counters may get slower as thread count grows
- padded counters should reduce cache-line fighting
- the result depends strongly on CPU and thread placement

This benchmark is about memory layout plus threads.

### 3. atomic_counter

Question:

> How expensive is shared synchronization?

What it measures:

- multiple threads
- shared counter variants
- mutex counter
- atomic counter
- relaxed atomic counter
- sharded counter

Core idea:

A shared counter looks tiny in source code, but it can become a serialization
point. If all threads update the same counter, they are all competing for the
same memory location. A mutex makes the serialization obvious. An atomic can be
lighter, but it still coordinates between cores. A sharded counter avoids most
of the sharing by giving each thread its own counter and summing at the end.

Expected shape:

- mutex counter should usually scale poorly
- atomic counter may be better but still contended
- relaxed atomic may be faster than default atomic ordering
- sharded counter should often scale best

This benchmark is about synchronization cost and contention.

### 4. thread_scaling

Question:

> What happens when I use more threads for the same workload?

What it measures:

- same workload
- 1, 2, 4, 8, 16 or more threads
- scaling limits
- oversubscription

Core idea:

More threads are not automatically faster. If the workload is CPU-bound and
parallel, speed may improve until you run out of useful cores. If the workload is
memory-bandwidth-bound, adding threads may hit a wall early. If you create more
threads than the machine can run at once, the scheduler has to switch between
them, and performance can flatten or get worse.

Expected shape:

- throughput rises at first
- then it flattens
- it may drop after oversubscription

This benchmark is about scalability.

## How The Four Differ

| Benchmark | Threads | Shared state | Main bottleneck | Main lesson |
| --- | --- | --- | --- | --- |
| `cache_stride` | 1 | no | cache misses / memory access pattern | locality matters |
| `false_sharing` | many | accidentally, through cache lines | cache coherence | layout matters |
| `atomic_counter` | many | yes, one logical counter | synchronization contention | sharing costs |
| `thread_scaling` | many | ideally no | CPU cores, memory bandwidth, scheduler | more threads have limits |

The big distinction:

- `cache_stride` asks how one thread uses memory.
- `false_sharing` asks how memory layout affects many threads.
- `atomic_counter` asks what happens when many threads intentionally share.
- `thread_scaling` asks how far parallel work scales.

## What To Write Before Running

For each benchmark, write one sentence before you run it:

```text
I expect variant A to be faster than variant B because ...
```

Then write one sentence after:

```text
The result did/did not match my expectation. I think this happened because ...
```

This is the habit that turns benchmark output into learning.

## What Counts As Success

You are learning the right thing when you can explain:

- what was measured
- what was not measured
- which variable changed
- which variables stayed constant
- why the faster result was faster
- what could make the result change on another machine
