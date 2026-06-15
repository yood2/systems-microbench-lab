# 02 CLI, Results, and Harness

## Goal

Give every benchmark the same basic shape:

```sh
./build/benchmarks/cache_stride --size-mb 512 --iterations 5 --output results/cache_stride.csv
```

The project may use Google Benchmark or a small custom harness. Since the goal is learning C++, a small custom harness is a good first choice.

## Required CSV Schema

Keep these columns stable:

```csv
benchmark,variant,input_size,threads,iterations,duration_ms,throughput_ops_sec,notes
```

Not every benchmark has a meaningful throughput value. Leave it empty or write `0`, but do not change the columns per benchmark.

## Minimal CLI Options

Every benchmark should support:

- `--iterations`
- `--output`

Benchmark-specific options can include:

- `--size-mb`
- `--threads`
- `--max-threads`
- `--block-size`
- `--file`

## Suggested Shared Helpers

Only extract these after you have a working benchmark:

- `include/bench_timer.h`: timer based on `std::chrono::steady_clock`
- `include/stats.h`: min, max, mean, maybe median
- `include/csv_writer.h`: stable CSV output
- `include/cli.h`: tiny argument parser

You do not need a perfect framework. You need enough shared code that each benchmark stays readable.

## Dead-Code Elimination Guard

Benchmarks must consume their results so the compiler cannot remove the work.

Ideas:

- accumulate a checksum
- print or store the checksum in the `notes` column
- write through a `volatile` sink sparingly
- use Google Benchmark utilities if you choose Google Benchmark

Prefer a checksum first because it is easy to understand.

## Checkpoint

You are done when one benchmark:

- accepts `--iterations`
- accepts `--output`
- writes the required CSV columns
- uses a timer
- records something that prevents accidental dead-code elimination

## Questions To Ask Yourself

- What does one "operation" mean for this benchmark?
- Is throughput meaningful here?
- If I run the benchmark twice, are the numbers roughly in the same neighborhood?
- If the input size changes, do the results change in a believable direction?
