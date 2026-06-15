# 04 Cache Stride

## Goal

Demonstrate cache locality and memory access patterns by scanning a large array with different strides.

Minimum variants:

- stride 1
- stride 2
- stride 4
- stride 8
- stride 16
- stride 64
- stride 256

Example command:

```sh
./build/benchmarks/cache_stride --size-mb 512 --iterations 5 --output results/cache_stride.csv
```

## Hypothesis

Sequential access should be faster than large-stride access because hardware prefetching and cache lines reward locality.

Write your own hypothesis before running the benchmark.

## Implementation Hints

Use a large `std::vector` of integers or 64-bit values. For each stride, walk the vector and accumulate a checksum.

Think carefully about what you want to hold constant:

- total array size
- number of iterations
- whether each variant performs the same number of loop steps
- whether each variant touches the same amount of memory

There are two valid experiment styles:

- fixed array scan: larger strides do fewer loads
- fixed load count: larger strides jump around more

Pick one, document it, and avoid mixing interpretations.

## C++ Concepts To Practice

- `std::vector`
- integer types from `<cstdint>`
- command-line parsing
- `std::chrono::steady_clock`
- avoiding signed/unsigned mistakes
- writing CSV rows

## Result Rows

Use variants like:

```csv
cache_stride,stride_1,512MB,1,5,123.4,1000000,checksum=...
cache_stride,stride_64,512MB,1,5,912.8,135000,checksum=...
```

## Explanation Prompts

Answer these in your notes:

- Which stride was fastest?
- Where did performance start to drop?
- Does the result suggest cache line effects?
- Did `perf stat` show more cache misses for large strides?
- What changed when the size exceeded cache capacity?

## Checkpoint

This benchmark is done when it runs all required strides, writes CSV, and includes a short explanation in `docs/results.md` or `docs/benchmark-notes.md`.
