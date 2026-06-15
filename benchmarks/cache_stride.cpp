#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

// This file is intentionally a scaffold.
//
// The CLI parsing and CSV plumbing are provided so you can focus on the core
// benchmark logic. Search for TODO(student) and implement those pieces yourself.

volatile std::uint64_t g_sink = 0;

struct Options {
  std::size_t size_mb = 512;
  int iterations = 5;
  std::string output_path = "results/cache_stride.csv";
  bool help = false;
};

struct BenchmarkResult {
  std::size_t stride = 0;
  double duration_ms = 0.0;
  double throughput_ops_sec = 0.0;
  std::uint64_t checksum = 0;
  std::string notes = "TODO: implement benchmark logic";
};

void print_usage(std::string_view program_name) {
  std::cout
      << "Usage: " << program_name
      << " [--size-mb N] [--iterations N] [--output PATH]\n\n"
      << "Examples:\n"
      << "  " << program_name
      << " --size-mb 512 --iterations 5 --output results/cache_stride.csv\n"
      << "  " << program_name << " --size-mb 64 --iterations 2\n";
}

std::size_t parse_positive_size(std::string_view text,
                                std::string_view flag_name) {
  try {
    std::size_t parsed_chars = 0;
    const std::size_t value = std::stoull(std::string(text), &parsed_chars);

    if (parsed_chars != text.size() || value == 0) {
      throw std::invalid_argument("not a positive integer");
    }

    return value;
  } catch (const std::exception &) {
    throw std::invalid_argument(std::string(flag_name) +
                                " must be a positive integer");
  }
}

Options parse_args(int argc, char **argv) {
  Options options;

  for (int i = 1; i < argc; ++i) {
    const std::string_view arg = argv[i];

    if (arg == "--help" || arg == "-h") {
      options.help = true;
      return options;
    }

    auto require_value = [&](std::string_view flag) -> std::string_view {
      if (i + 1 >= argc) {
        throw std::invalid_argument(std::string(flag) + " requires a value");
      }

      ++i;
      return argv[i];
    };

    if (arg == "--size-mb") {
      options.size_mb = parse_positive_size(require_value(arg), arg);
    } else if (arg == "--iterations") {
      const std::size_t value = parse_positive_size(require_value(arg), arg);

      if (value > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("--iterations is too large");
      }

      options.iterations = static_cast<int>(value);
    } else if (arg == "--output") {
      options.output_path = std::string(require_value(arg));
    } else {
      throw std::invalid_argument("unknown option: " + std::string(arg));
    }
  }

  return options;
}

std::size_t element_count_for_size_mb(std::size_t size_mb) {
  const std::size_t bytes = size_mb * 1024ULL * 1024ULL;
  return bytes / sizeof(std::uint64_t);
}

std::vector<std::uint64_t> make_data(std::size_t element_count) {
  std::vector<std::uint64_t> data(element_count);

  // TODO(student):
  // Fill data with deterministic non-zero values.
  //
  // Why?
  // - The benchmark needs memory to read.
  // - A checksum over the reads helps prove the work really happened.
  //
  // Hint:
  // for (std::size_t i = 0; i < data.size(); ++i) {
  //   data[i] = ...;
  // }

  return data;
}

[[maybe_unused]] std::uint64_t
scan_with_stride(const std::vector<std::uint64_t> &data, std::size_t stride) {
  std::uint64_t checksum = 0;

  // TODO(student):
  // Read the vector using the requested stride and add each loaded value to
  // checksum.
  //
  // Design choice:
  // Decide whether each stride should:
  // 1. touch fewer elements as stride grows, or
  // 2. touch every element once but in stride-shaped order.
  //
  // The lesson doc recommends option 2 because each variant does the same
  // number of loads. That makes the timing comparison easier to interpret.
  //
  // Hint for option 2:
  // for each offset from 0 to stride - 1:
  //   for each index starting at offset and increasing by stride:
  //     add data[index] to checksum

  (void)data;
  (void)stride;
  return checksum;
}

BenchmarkResult run_one_stride(const std::vector<std::uint64_t> &data,
                               std::size_t stride, int iterations) {
  BenchmarkResult result;
  result.stride = stride;

  // TODO(student):
  // Time repeated calls to scan_with_stride.
  //
  // Suggested steps:
  // 1. Capture start time with std::chrono::steady_clock::now().
  // 2. Run scan_with_stride(data, stride) once per iteration.
  // 3. Add each scan result into result.checksum.
  // 4. Capture end time.
  // 5. Convert elapsed time to milliseconds.
  // 6. Compute throughput as total loads divided by elapsed seconds.
  // 7. Store the checksum in g_sink so the compiler cannot ignore the work.
  //
  // Headers you will need when you implement timing:
  // #include <chrono>

  (void)data;
  (void)iterations;
  g_sink = result.checksum;
  return result;
}

void ensure_parent_directory_exists(const std::string &path) {
  const std::filesystem::path output_path(path);
  const std::filesystem::path parent = output_path.parent_path();

  if (!parent.empty()) {
    std::filesystem::create_directories(parent);
  }
}

void write_csv_header(std::ofstream &output) {
  output << "benchmark,variant,input_size,threads,iterations,duration_ms,"
            "throughput_ops_sec,notes\n";
}

void write_csv_row(std::ofstream &output, std::size_t size_mb, int iterations,
                   const BenchmarkResult &result) {
  output << "cache_stride,stride_" << result.stride << ',' << size_mb
         << "MB,1," << iterations << ',' << std::fixed << std::setprecision(3)
         << result.duration_ms << ',' << std::setprecision(2)
         << result.throughput_ops_sec << ",checksum=" << result.checksum << ' '
         << result.notes << '\n';
}

} // namespace

int main(int argc, char **argv) {
  try {
    const Options options = parse_args(argc, argv);

    if (options.help) {
      print_usage(argv[0]);
      return EXIT_SUCCESS;
    }

    const std::size_t element_count = element_count_for_size_mb(options.size_mb);

    if (element_count == 0) {
      std::cerr << "--size-mb is too small for this benchmark\n";
      return EXIT_FAILURE;
    }

    std::cout << "cache_stride scaffold: " << options.size_mb << " MB, "
              << options.iterations << " iterations\n";
    std::cout << "Implement TODO(student) sections before trusting results.\n";

    const std::vector<std::uint64_t> data = make_data(element_count);
    const std::vector<std::size_t> strides = {1, 2, 4, 8, 16, 64, 256};

    ensure_parent_directory_exists(options.output_path);
    std::ofstream output(options.output_path);

    if (!output) {
      std::cerr << "failed to open output file: " << options.output_path
                << '\n';
      return EXIT_FAILURE;
    }

    write_csv_header(output);

    for (const std::size_t stride : strides) {
      const BenchmarkResult result =
          run_one_stride(data, stride, options.iterations);

      write_csv_row(output, options.size_mb, options.iterations, result);

      std::cout << "  stride " << std::setw(3) << stride
                << ": TODO implementation pending\n";
    }

    std::cout << "wrote scaffold CSV to " << options.output_path << '\n';
    return EXIT_SUCCESS;
  } catch (const std::exception &error) {
    std::cerr << "error: " << error.what() << "\n\n";
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }
}
