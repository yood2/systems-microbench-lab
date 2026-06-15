# 01 Repo Layout and CMake

## Goal

Create the project shape and build system before writing benchmark logic.

Suggested structure:

```text
systems-microbench-lab/
  CMakeLists.txt
  README.md
  benchmarks/
  include/
  scripts/
  docs/
  results/
  plots/
```

## What To Build

Start with one tiny benchmark executable, then add more executables as you go.

Your first CMake target can be something like:

- executable name: `cache_stride`
- source file: `benchmarks/cache_stride.cpp`
- C++ standard: 20
- Release optimization: `-O3 -DNDEBUG`

Avoid adding a library too early. Once you have duplicated helper code in two or three benchmarks, move shared code into `include/`.

## CMake Checklist

- Require CMake 3.20 or newer.
- Set `CMAKE_CXX_STANDARD 20`.
- Set `CMAKE_CXX_STANDARD_REQUIRED ON`.
- Export `compile_commands.json`.
- Add warnings such as `-Wall -Wextra -Wpedantic` for Clang/GCC.
- Keep benchmark executables in `build/benchmarks` if you want paths to match the spec.

## Learning Focus

Pay attention to these C++ and build concepts:

- translation units
- headers vs source files
- compiler flags
- Release vs Debug builds
- target-specific CMake settings

## Checkpoint

You are done with this lesson when:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

succeeds and produces at least one runnable benchmark executable.

## Common Pitfalls

- Running benchmarks from a Debug build and trusting the numbers.
- Adding many abstractions before the first benchmark works.
- Forgetting that CMake configure and build are separate steps.
- Hardcoding paths that only work from one current working directory.
