#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$repo_root/build"
results_dir="$repo_root/results"

"$repo_root/scripts/build.sh" "$build_dir"

mkdir -p "$results_dir"

"$build_dir/benchmarks/cache_stride" \
  --size-mb "${CACHE_STRIDE_SIZE_MB:-512}" \
  --iterations "${CACHE_STRIDE_ITERATIONS:-5}" \
  --output "$results_dir/cache_stride.csv"

echo "run_all complete"
