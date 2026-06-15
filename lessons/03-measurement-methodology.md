# 03 Measurement Methodology

## Goal

Make your numbers defensible. Microbenchmarks are useful only when the experiment is clear and the caveats are visible.

## Machine Info To Capture

Create `docs/methodology.md` once you have the MVP running. Include:

- CPU model
- core count
- RAM
- OS and kernel
- compiler and version
- CMake version
- build type and compiler flags

Useful commands on Linux:

```sh
lscpu
uname -a
g++ --version
clang++ --version
cmake --version
free -h
```

## Run Discipline

For each benchmark:

- use Release mode
- run multiple iterations
- close noisy background work if possible
- keep input sizes large enough to measure
- write down the exact command
- keep raw CSV files

## Profiling Requirement

At least two benchmarks need `perf stat` or `perf record` output.

Start simple:

```sh
perf stat ./build/benchmarks/cache_stride --size-mb 512 --iterations 5 --output results/cache_stride.csv
```

Good events to inspect:

- cycles
- instructions
- cache-references
- cache-misses
- branches
- branch-misses
- context-switches

## Caveats To Document

Every benchmark writeup should mention possible caveats:

- CPU frequency scaling
- thermal throttling
- scheduler noise
- different CPU architectures
- NUMA effects
- compiler optimizations
- OS page cache effects for file benchmarks

## Checkpoint

You are done when `docs/methodology.md` explains how results were produced and what could make them vary.
