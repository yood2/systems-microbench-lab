#!/usr/bin/env bash
set -euo pipefail

if [[ "$#" -lt 1 ]]; then
  echo "usage: $0 PROGRAM [ARGS...]" >&2
  echo "example: $0 ./build/benchmarks/cache_stride --size-mb 512 --iterations 5" >&2
  exit 2
fi

if ! command -v perf >/dev/null 2>&1; then
  echo "error: perf was not found. This helper is intended for Linux." >&2
  exit 1
fi

perf stat \
  -e cycles,instructions,cache-references,cache-misses,branches,branch-misses,context-switches \
  "$@"
