#include <filesystem>
#include <iostream>

#include "inspect.hpp"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: bininspect <file>\n";
    return 1;
  }

  std::filesystem::path file = argv[1];

  if (!std::filesystem::exists(file)) {
    std::cerr << "Error: file does not exist\n";
    return 1;
  }

  try {
    bininspect::inspect_file(file);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
