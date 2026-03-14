#include <filesystem>
#include <iostream>

#include "inspect.hpp"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: binfo <file>\n";
    return 1;
  }

  try {
    std::filesystem::path path = argv[1];

    if (!std::filesystem::exists(path)) {
      std::cerr << "Error: file does not exist\n";
      return 1;
    }

    binfo::inspect(binfo::BinaryFile{path});
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
