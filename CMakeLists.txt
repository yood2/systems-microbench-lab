cmake_minimum_required(VERSION 3.20)

project(systems_microbench_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(add_microbench name)
  add_executable(${name} benchmarks/${name}.cpp)

  target_include_directories(${name}
    PRIVATE
      ${PROJECT_SOURCE_DIR}/include
  )

  target_compile_options(${name}
    PRIVATE
      -Wall
      -Wextra
      -Wpedantic
  )

  set_target_properties(${name}
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks
  )
endfunction()

# add benchmarks here
add_microbench(cache_stride)