#include <chrono>
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

volatile std::uint64_t g_sink = 0;

struct Options {
  std::size_t size_mb = 512;
  int iterations = 5;
  std::string output_path = "results/cache_stride.csv";
  bool help = false;
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

std::size_t parse_size(std::string_view text, std::string_view flag_name) {
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
      options.size_mb = parse_size(require_value(arg), arg);
    } else if (arg == "--iterations") {
      const std::size_t value = parse_size(require_value(arg), arg);

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

std::vector<std::uint64_t> make_data(std::size_t element_count) {
  std::vector<std::uint64_t> data(element_count);

  for (std::size_t i = 0; i < data.size(); ++i) {
    data[i] = static_cast<std::uint64_t>(i * 1315423911ULL);
  }

  return data;
}

std::uint64_t scan_with_stride(const std::vector<std::uint64_t> &data,
                               std::size_t stride) {
  std::uint64_t checksum = 0;

  // This touches every element once, but changes the access pattern. stride=1
  // is sequential; larger strides jump farther between consecutive loads.
  for (std::size_t offset = 0; offset < stride && offset < data.size();
       ++offset) {
    for (std::size_t i = offset; i < data.size(); i += stride) {
      checksum += data[i];
    }
  }

  return checksum;
}

void ensure_parent_directory_exists(const std::string &path) {
  const std::filesystem::path output_path(path);
  const std::filesystem::path parent = output_path.parent_path();

  if (!parent.empty()) {
    std::filesystem::create_directories(parent);
  }
}

} // namespace

int main(int argc, char **argv) {
  try {
    const Options options = parse_args(argc, argv);

    if (options.help) {
      print_usage(argv[0]);
      return EXIT_SUCCESS;
    }

    const std::size_t bytes = options.size_mb * 1024ULL * 1024ULL;
    const std::size_t element_count = bytes / sizeof(std::uint64_t);

    if (element_count == 0) {
      std::cerr << "--size-mb is too small for this benchmark\n";
      return EXIT_FAILURE;
    }

    std::cout << "cache_stride: " << options.size_mb << " MB, "
              << options.iterations << " iterations\n";

    const std::vector<std::uint64_t> data = make_data(element_count);
    const std::vector<std::size_t> strides = {1, 2, 4, 8, 16, 64, 256};

    ensure_parent_directory_exists(options.output_path);
    std::ofstream output(options.output_path);

    if (!output) {
      std::cerr << "failed to open output file: " << options.output_path
                << '\n';
      return EXIT_FAILURE;
    }

    output << "benchmark,variant,input_size,threads,iterations,duration_ms,"
              "throughput_ops_sec,notes\n";

    for (const std::size_t stride : strides) {
      std::uint64_t checksum = 0;

      const auto start = std::chrono::steady_clock::now();

      for (int iteration = 0; iteration < options.iterations; ++iteration) {
        checksum += scan_with_stride(data, stride);
      }

      const auto end = std::chrono::steady_clock::now();
      const std::chrono::duration<double, std::milli> elapsed_ms = end - start;
      const double elapsed_seconds = elapsed_ms.count() / 1000.0;
      const double operations =
          static_cast<double>(element_count) * options.iterations;
      const double throughput =
          elapsed_seconds > 0.0 ? operations / elapsed_seconds : 0.0;

      g_sink = checksum;

      output << "cache_stride,stride_" << stride << ',' << options.size_mb
             << "MB,1," << options.iterations << ',' << std::fixed
             << std::setprecision(3) << elapsed_ms.count() << ','
             << std::setprecision(2) << throughput << ",checksum="
             << checksum << '\n';

      std::cout << "  stride " << std::setw(3) << stride << ": "
                << std::fixed << std::setprecision(3) << elapsed_ms.count()
                << " ms, " << std::setprecision(2) << throughput
                << " loads/sec\n";
    }

    std::cout << "wrote " << options.output_path << '\n';
    return EXIT_SUCCESS;
  } catch (const std::exception &error) {
    std::cerr << "error: " << error.what() << "\n\n";
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }
}
