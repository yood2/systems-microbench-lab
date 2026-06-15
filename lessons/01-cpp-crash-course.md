# C++ Crash Course for This Project

This is not a full C++ course. It is the minimum C++ you need to understand the
benchmark code in this repo.

## What C++ Builds Into

C++ source code is compiled into machine code.

Typical flow:

```text
.cpp file -> compiler -> object file -> linker -> executable
```

In this project, CMake hides most of those steps, but they still exist.

## A Tiny Program

```cpp
#include <iostream>

int main() {
  std::cout << "hello\n";
  return 0;
}
```

Important pieces:

- `#include <iostream>` imports declarations from the standard library.
- `int main()` is where the program starts.
- `std::cout` writes to standard output.
- `return 0` means success.

## Namespaces

Most standard library names live in the `std` namespace:

```cpp
std::vector<int> values;
std::string name;
std::cout << "hi\n";
```

You will see `namespace { ... }` in this project. That creates an unnamed
namespace, which means the helper functions inside are private to that `.cpp`
file. It keeps small helper functions from leaking into the rest of the program.

## Basic Types

Common types in this repo:

```cpp
int                     // normal signed integer
std::size_t             // size or index, unsigned
std::uint64_t           // exactly 64-bit unsigned integer
double                  // floating point number
bool                    // true or false
std::string             // owned text
std::string_view        // non-owning view of text
```

Why use `std::uint64_t` in benchmarks?

- It has a known size.
- It is good for counters and checksums.
- It makes memory-size math easier to reason about.

## Variables

```cpp
std::size_t count = 1024;
double duration_ms = 12.5;
bool ok = true;
```

Use `const` when a value should not change:

```cpp
const std::size_t stride = 64;
```

This helps both the compiler and the reader.

## Vectors

`std::vector<T>` is a resizable array of `T`.

```cpp
std::vector<std::uint64_t> data(1000);

data[0] = 42;
std::size_t n = data.size();
```

For this project, `std::vector` is the main way to allocate memory for array
benchmarks.

## Loops

Index loop:

```cpp
for (std::size_t i = 0; i < data.size(); ++i) {
  data[i] = i;
}
```

Range loop:

```cpp
for (const std::size_t stride : strides) {
  std::cout << stride << "\n";
}
```

Use index loops when the index matters. Use range loops when you just need each
value.

## Functions

```cpp
std::uint64_t add_one(std::uint64_t value) {
  return value + 1;
}
```

Function shape:

```text
return_type function_name(parameter_type parameter_name)
```

Functions make benchmark code easier to test mentally:

- one function parses CLI options
- one function creates input data
- one function runs a variant
- one function writes CSV

## Passing By Value vs Reference

Passing by value copies:

```cpp
void f(std::vector<int> data); // copies the whole vector
```

Passing by const reference avoids copying:

```cpp
void f(const std::vector<int> &data); // borrows without modifying
```

Most benchmark functions should take large vectors by `const &`.

## Structs

A struct groups related fields:

```cpp
struct Options {
  std::size_t size_mb = 512;
  int iterations = 5;
};
```

This is clearer than passing many loose variables around.

## Timing With chrono

Use `std::chrono::steady_clock` for elapsed time:

```cpp
const auto start = std::chrono::steady_clock::now();

// measured work goes here

const auto end = std::chrono::steady_clock::now();
const std::chrono::duration<double, std::milli> elapsed_ms = end - start;
```

`steady_clock` is appropriate because it is monotonic. It will not jump backward
if the system clock changes.

## Files and Streams

Write a file:

```cpp
std::ofstream output("results/example.csv");

if (!output) {
  std::cerr << "failed to open output\n";
  return 1;
}

output << "column_a,column_b\n";
output << "hello,123\n";
```

`std::cout` is normal output. `std::cerr` is error output.

## Exceptions

This project uses exceptions for CLI parsing errors:

```cpp
throw std::invalid_argument("--size-mb requires a value");
```

Then `main` catches the error:

```cpp
try {
  // program
} catch (const std::exception &error) {
  std::cerr << error.what() << "\n";
  return 1;
}
```

You do not need to love exceptions yet. For beginner CLI code, this is a compact
way to report invalid arguments.

## RAII

RAII means resources are acquired and released by object lifetime.

Examples:

- `std::vector` allocates memory and frees it when it goes out of scope.
- `std::ofstream` opens a file and closes it when it goes out of scope.
- `std::lock_guard` locks a mutex and unlocks it when it goes out of scope.

This is one of the most important C++ ideas.

## Threads Preview

You will use this later:

```cpp
#include <thread>

std::thread worker([] {
  // code running on another thread
});

worker.join();
```

Always join threads you create, unless you have a very specific reason not to.

## Atomics Preview

You will use this in `atomic_counter`:

```cpp
#include <atomic>

std::atomic<std::uint64_t> counter = 0;
counter.fetch_add(1);
```

Atomics let multiple threads safely operate on the same variable, but they are
not free. Measuring that cost is the point of `atomic_counter`.

## Beginner Build Loop

Use this loop constantly:

```sh
./scripts/build.sh
```

When you get compiler errors, read the first error first. Later errors are often
caused by the first one.

## What To Learn First

For `cache_stride`, focus on:

- `std::vector`
- `std::size_t`
- `for` loops
- functions
- `const &`
- `std::chrono`
- file output

Do not worry about templates, inheritance, advanced move semantics, or custom
allocators yet.
