# Lessons

These lessons are meant to teach first and guide implementation second. The
benchmark source files should feel like exercises, not answers.

## Start Here

1. [Benchmarking Fundamentals and the MVP Benchmarks](./00-benchmarking-fundamentals-and-mvp.md)
2. [C++ Crash Course for This Project](./01-cpp-crash-course.md)
3. [CMake Crash Course](./02-cmake-crash-course.md)
4. [Cache Stride Implementation Guide](./03-cache-stride-scaffold-guide.md)

After that, use the older checklist-style notes as project guides:

1. [Project Roadmap](./00-project-roadmap.md)
2. [Repo Layout and CMake Checklist](./01-repo-layout-and-cmake.md)
3. [CLI, Results, and Harness](./02-cli-results-and-harness.md)
4. [Measurement Methodology](./03-measurement-methodology.md)
5. [Cache Stride Notes](./04-cache-stride.md)
6. [False Sharing Notes](./05-false-sharing.md)
7. [Atomic Counter Notes](./06-atomic-counter.md)
8. [Thread Scaling Notes](./07-thread-scaling.md)
9. [Full Benchmark Set](./08-full-benchmark-set.md)
10. [Results, Plots, and Writeups](./09-results-plots-and-writeups.md)
11. [Polish and Stretch Goals](./10-polish-and-stretch-goals.md)

## How To Use These

For each benchmark:

- Read the concept lesson before touching code.
- Write down your hypothesis in plain English.
- Implement one TODO at a time.
- Build after every small change.
- Run with tiny inputs first.
- Only trust numbers from a Release build.
- Record what surprised you.

The MVP is `cache_stride`, `false_sharing`, `atomic_counter`, and
`thread_scaling`. The point is not just to get numbers. The point is to learn
why those numbers change.
