# 00 Project Roadmap

## Goal

Build a C++20 microbenchmark suite that demonstrates how systems-level behavior affects performance:

- caches and memory access patterns
- cache line contention
- atomics and locks
- allocation and object reuse
- thread scaling and oversubscription
- buffered file I/O and `mmap`

The final repo should be understandable to another engineer: build commands, run commands, CSV results, plots, methodology, and explanations.

## Constraints

- Use C++20.
- Use CMake.
- Linux is required for final results.
- macOS is fine for quick smoke tests.
- Do not use any internal, proprietary, or company-specific data.
- Prefer a small implementation over a large framework.

## MVP Scope

Implement these first:

1. `cache_stride`
2. `false_sharing`
3. `atomic_counter`
4. `thread_scaling`

Also include:

- one command to build
- one command to run all MVP benchmarks
- stable CSV output
- a basic README
- one `perf stat` example
- one short results writeup

## Full Scope

After the MVP:

- add 2-4 more benchmarks
- generate at least 3 plots
- write `docs/methodology.md`
- write `docs/results.md`
- include `perf` analysis for at least 2 benchmarks
- polish the README so it explains what the project teaches

## A Good Implementation Rhythm

For each benchmark:

1. State the hypothesis.
2. Implement one minimal variant.
3. Add the comparison variant.
4. Add CLI options.
5. Emit CSV.
6. Run multiple iterations.
7. Check that the compiler cannot optimize away the work.
8. Record results.
9. Explain why the result happened.
10. Write down caveats.

## Definition Of Done

You should be able to run:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./scripts/run_all.sh
```

and produce result files under `results/`.
