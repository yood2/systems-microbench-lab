# 10 Polish and Stretch Goals

## Goal

Make the project shareable after the core benchmarks work.

## README Polish

The README should include:

- what the project is
- what each benchmark demonstrates
- how to build
- how to run all benchmarks
- how to run one benchmark
- example CSV output
- summary of interesting findings
- link to `docs/results.md`

## Script Polish

Add:

- `scripts/run_all.sh`
- `scripts/run_perf.sh`
- `scripts/plot.py`

The scripts should create output directories if needed and fail clearly when a benchmark executable is missing.

## GitHub Actions

A useful CI check:

- configure CMake
- build in Release mode
- optionally run a tiny smoke test with small input sizes

Do not run heavy benchmarks in CI.

## Dockerfile

A Dockerfile can make Linux builds easier from macOS.

Keep it simple:

- install compiler
- install CMake
- install Python plotting dependencies if needed
- build the project

## CPU Affinity

CPU affinity can reduce scheduler noise and make some thread benchmarks more reproducible.

Learn:

- `sched_setaffinity`
- CPU sets
- how pinning can help
- how pinning can accidentally bias results

## Larger Experiments

Only add these after the main project is solid:

- NUMA experiment on a multi-socket machine
- x86 vs Graviton comparison
- flamegraphs
- lessons learned mapped to real production performance issues

## Final Check

The project is polished when it is easy to answer:

- What did you measure?
- How did you measure it?
- What did you learn?
- What would change on another machine?
- How can someone else reproduce it?
