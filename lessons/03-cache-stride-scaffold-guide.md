# Cache Stride Implementation Guide

This guide matches the TODOs in `benchmarks/cache_stride.cpp`.

The file intentionally gives you:

- CLI parsing
- output path handling
- CSV header and row writing
- a list of required strides
- places to implement the real benchmark logic

Your job is to implement the memory setup, scan, timing, and throughput math.

## Requirements

The benchmark should support:

```sh
./build/benchmarks/cache_stride --size-mb 512 --iterations 5 --output results/cache_stride.csv
```

It should test strides:

```text
1, 2, 4, 8, 16, 64, 256
```

It should write CSV columns:

```csv
benchmark,variant,input_size,threads,iterations,duration_ms,throughput_ops_sec,notes
```

## Step 1: Understand The Data Size

The scaffold turns megabytes into element count:

```cpp
const std::size_t bytes = size_mb * 1024ULL * 1024ULL;
const std::size_t element_count = bytes / sizeof(std::uint64_t);
```

If `size_mb` is 512 and each element is 8 bytes, the vector has:

```text
512 * 1024 * 1024 / 8 = 67,108,864 elements
```

That is large enough to exceed most CPU caches, which is the point.

## Step 2: Fill The Vector

Implement:

```cpp
std::vector<std::uint64_t> make_data(std::size_t element_count)
```

Goal:

- allocate a vector
- fill every element with a deterministic value
- return the vector

Why deterministic?

If the values are predictable, your checksum should be stable across runs with
the same input.

Possible shape:

```cpp
for (...) {
  data[i] = some_value_based_on_i;
}
```

Do not use random numbers yet. Randomness adds another moving part.

## Step 3: Choose The Stride Experiment Style

There are two common styles.

Style A:

```text
for i = 0; i < size; i += stride
```

This is easy, but larger strides do fewer loads. That means stride 256 performs
much less work than stride 1.

Style B:

```text
for offset = 0; offset < stride; ++offset
  for i = offset; i < size; i += stride
```

This still changes the access pattern, but touches every element once.

For learning, use Style B. It makes the comparison cleaner because every stride
does the same number of loads.

## Step 4: Implement scan_with_stride

Implement:

```cpp
std::uint64_t scan_with_stride(const std::vector<std::uint64_t> &data,
                               std::size_t stride)
```

Rules:

- do not modify `data`
- add loaded values into `checksum`
- return `checksum`

Questions to ask yourself:

- Does stride 1 touch every element?
- Does stride 2 touch every element?
- Does stride 256 touch every element?
- Does the function behave correctly if `stride` is larger than `data.size()`?

## Step 5: Time One Variant

Implement:

```cpp
BenchmarkResult run_one_stride(...)
```

Suggested shape:

```text
start timer
for each iteration:
  checksum += scan_with_stride(data, stride)
stop timer
duration_ms = elapsed time
throughput = total loads / elapsed seconds
store checksum
return result
```

Use:

```cpp
std::chrono::steady_clock::now()
```

You will need:

```cpp
#include <chrono>
```

## Step 6: Guard Against Fake Work

If the compiler proves that a calculation has no visible effect, it may remove
it. That is deadly for benchmarks.

The scaffold has:

```cpp
volatile std::uint64_t g_sink = 0;
```

After computing the checksum, assign it:

```cpp
g_sink = result.checksum;
```

This is not a perfect benchmarking technique, but it is simple and good enough
for this learning project.

## Step 7: Calculate Throughput

For this benchmark:

```text
operations = element_count * iterations
throughput = operations / elapsed_seconds
```

If you use Style B, every stride touches `element_count` elements per iteration.

If you choose Style A instead, this formula is wrong because each stride touches
a different number of elements.

## Step 8: Run Small First

Use tiny inputs while coding:

```sh
./scripts/build.sh
./build/benchmarks/cache_stride --size-mb 8 --iterations 1 --output results/cache_stride.csv
```

Then try the required-style run:

```sh
./build/benchmarks/cache_stride --size-mb 512 --iterations 5 --output results/cache_stride.csv
```

## Step 9: Interpret Results

Write down:

- Which stride was fastest?
- Which stride was slowest?
- Did larger strides generally get slower?
- Did the pattern change when you changed `--size-mb`?
- What caveats might explain weird results?

## Do Not Optimize Too Early

First make it correct and understandable.

Later you can experiment with:

- `int` vs `std::uint64_t`
- different array sizes
- different stride lists
- Linux `perf stat`
- plotting results

The goal of this first benchmark is not fancy C++. The goal is to connect memory
access patterns to measured performance.
