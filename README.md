# systems-microbench-lab

A C++20/Linux microbenchmark lab for learning how low-level systems behavior affects performance.

## Dev Setup

macOS smoke testing:

```sh
brew install cmake
```

Linux is required for final benchmark results and `perf` analysis.

## Learning Guides

Start with [lessons/README.md](lessons/README.md). The lesson files are written as implementation guidance, checkpoints, and hints so you can build the project mostly yourself.

## Build

```sh
./scripts/build.sh
```

This configures CMake into `build/` and builds the project in Release mode.

## Run

Run all implemented benchmarks:

```sh
./scripts/run_all.sh
```

Run the first benchmark directly:

```sh
./build/benchmarks/cache_stride --size-mb 512 --iterations 5 --output results/cache_stride.csv
```

For a quick smoke test:

```sh
CACHE_STRIDE_SIZE_MB=8 CACHE_STRIDE_ITERATIONS=1 ./scripts/run_all.sh
```

On Linux, run a benchmark with `perf stat`:

```sh
./scripts/run_perf.sh ./build/benchmarks/cache_stride --size-mb 512 --iterations 5 --output results/cache_stride.csv
```
