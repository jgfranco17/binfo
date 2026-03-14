#include "inspect.hpp"

#include <fstream>
#include <iostream>

namespace binfo {

void inspect_file(const std::filesystem::path &path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);

  if (!file) {
    throw std::runtime_error("Unable to open file");
  }

  auto size = file.tellg();

  std::cout << "File: " << path << "\n";
  std::cout << "Size: " << size << " bytes\n";
}

} // namespace binfo
