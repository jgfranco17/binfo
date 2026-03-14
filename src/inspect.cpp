#include "inspect.hpp"
#include "detect.hpp"

#include <fstream>
#include <iostream>

namespace binfo {

void inspect_file(const std::filesystem::path &path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);

  if (!file) {
    throw std::runtime_error("Unable to open file");
  }

  auto size = file.tellg();

  std::string format = detect_format(path);

  std::cout << "File: " << path << "\n";
  std::cout << "Size: " << size << " bytes\n";
  std::cout << "Format: " << format << "\n";
}

} // namespace binfo
