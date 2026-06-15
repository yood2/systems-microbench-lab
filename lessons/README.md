# Lessons

These lessons turn the project requirements into a learning path. They are intentionally guidance, not finished implementation. The goal is for you to write the C++ yourself while having enough structure to avoid getting lost.

## Suggested Order

1. [00 Project Roadmap](./00-project-roadmap.md)
2. [01 Repo Layout and CMake](./01-repo-layout-and-cmake.md)
3. [02 CLI, Results, and Harness](./02-cli-results-and-harness.md)
4. [03 Measurement Methodology](./03-measurement-methodology.md)
5. [04 Cache Stride](./04-cache-stride.md)
6. [05 False Sharing](./05-false-sharing.md)
7. [06 Atomic Counter](./06-atomic-counter.md)
8. [07 Thread Scaling](./07-thread-scaling.md)
9. [08 Full Benchmark Set](./08-full-benchmark-set.md)
10. [09 Results, Plots, and Writeups](./09-results-plots-and-writeups.md)
11. [10 Polish and Stretch Goals](./10-polish-and-stretch-goals.md)

## How To Use These

For each lesson:

- Read the goal and acceptance criteria first.
- Implement the smallest working version.
- Run it in Release mode.
- Write down what you expected before looking at the numbers.
- Add notes about caveats while the experiment is fresh.

The project is done in two passes:

- MVP: `cache_stride`, `false_sharing`, `atomic_counter`, `thread_scaling`, CSV output, basic README, one `perf stat` example, one short results writeup.
- Full version: 6-8 benchmarks, plots, methodology docs, results docs, and two or more profiling examples.
