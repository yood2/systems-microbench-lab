# CMake Crash Course

CMake is a build configuration tool. You describe your project in
`CMakeLists.txt`, and CMake generates files for the actual build system.

On this machine, the generated build system is likely Makefiles or Ninja files.
You usually do not edit those generated files.

## Source Directory vs Build Directory

This repo uses an out-of-source build:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Meaning:

- `-S .` says the source directory is the current directory.
- `-B build` says generated build files go in `build/`.
- `cmake --build build` compiles using the generated files.

`build/` is disposable. If it gets weird, you can remove it and configure again.

## What CMakeLists.txt Is For

The root `CMakeLists.txt` answers questions like:

- What is the project called?
- Which language standard should be used?
- Which `.cpp` files become executables?
- Which include directories should the compiler search?
- Which compiler warnings should be enabled?
- Where should built executables be placed?

It should not contain benchmark logic. It only describes how to build the code.

## Current Project CMake

The project starts like this:

```cmake
cmake_minimum_required(VERSION 3.20)

project(systems_microbench_lab LANGUAGES CXX)
```

`cmake_minimum_required` says which CMake features we assume are available.

`project(... LANGUAGES CXX)` says this is a C++ project.

## C++ Standard

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

This means:

- use C++20
- fail if the compiler cannot provide C++20
- avoid compiler-specific extensions by default

## compile_commands.json

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

This creates `build/compile_commands.json`, which helps editors and language
servers understand how each file is compiled.

## Targets

The most important CMake idea is the target.

A target is something CMake builds. For this project, each benchmark is an
executable target.

Example:

```cmake
add_executable(cache_stride benchmarks/cache_stride.cpp)
```

This says:

> Build an executable named `cache_stride` from `benchmarks/cache_stride.cpp`.

## Include Directories

```cmake
target_include_directories(cache_stride
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
```

This tells the compiler where to look for your project headers.

`PRIVATE` means this include path is needed only to build this target. That is
fine for executable targets.

## Compiler Warnings

```cmake
target_compile_options(cache_stride
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
```

Warnings help you catch suspicious code before it becomes a bug.

Different compilers support different flags, but these are common on Clang and
GCC.

## Output Directory

```cmake
set_target_properties(cache_stride
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks
)
```

This places the built executable at:

```text
build/benchmarks/cache_stride
```

That matches the command style in the requirements.

## The Helper Function

The repo uses a small helper:

```cmake
function(add_microbench name)
  add_executable(${name} benchmarks/${name}.cpp)
  ...
endfunction()
```

Then:

```cmake
add_microbench(cache_stride)
```

This avoids repeating the same CMake setup for every benchmark.

Important:

```cmake
add_microbench(cache_stride)
```

expects this file to exist:

```text
benchmarks/cache_stride.cpp
```

Later, when you implement `false_sharing.cpp`, you can uncomment:

```cmake
add_microbench(false_sharing)
```

## Configure vs Build

Configure:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

This reads `CMakeLists.txt` and prepares the build directory.

Build:

```sh
cmake --build build
```

This compiles and links the code.

If you add a new source file and update `CMakeLists.txt`, CMake may reconfigure
automatically during the next build. If things look odd, run the configure
command again.

## Release vs Debug

For performance numbers, use Release:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

Release builds enable optimizations. Debug builds are better for debugging but
not for benchmark results.

## The scripts/build.sh Wrapper

Instead of typing both commands every time, use:

```sh
./scripts/build.sh
```

That script runs configure and build for you.

## Common Beginner Mistakes

- Editing files inside `build/`.
- Forgetting to add a new `.cpp` file to CMake.
- Running benchmark numbers from a Debug build.
- Assuming CMake compiles code during configure.
- Treating `CMakeLists.txt` like a shell script.

## Mental Model

Think of CMake like this:

```text
CMakeLists.txt describes targets
cmake -S . -B build generates build files
cmake --build build compiles targets
build/benchmarks/cache_stride is the executable
```
