# 09 Results, Plots, and Writeups

## Goal

Turn raw benchmark output into something another engineer can inspect.

Required docs:

- `docs/methodology.md`
- `docs/results.md`
- `docs/benchmark-notes.md`

Required artifacts:

- CSV or JSON for every benchmark
- at least 3 generated plots for the full project
- `perf` output for at least 2 benchmarks

## Plotting

Create `scripts/plot.py` once you have a few CSV files.

Good first plots:

- cache stride vs duration
- false sharing adjacent vs padded
- thread count vs throughput

Keep plot generation boring and repeatable. A simple Python script that reads CSV and writes PNG files is enough.

## Benchmark Writeup Template

Use this shape for every benchmark:

```md
### Benchmark Name

Hypothesis:

Command:

Result:

Explanation:

Caveats:
```

## Results Discipline

For every result, include:

- exact command
- machine details or link to methodology
- date of run
- build type
- short interpretation

## Interesting Findings

Your README should eventually summarize the most interesting findings. These should be concrete, for example:

- stride 64 was much slower than stride 1 on a 512 MB array
- padded counters were several times faster than adjacent counters at 8 threads
- a shared atomic counter stopped scaling after a small number of threads

Avoid claiming universal truths. Say what happened on your machine and why it probably happened.

## Checkpoint

This lesson is done when someone can read the README, follow links to results, and understand both the numbers and the caveats.
