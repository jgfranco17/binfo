#include "inspect.hpp"
#include "detect.hpp"

#include <fstream>
#include <iostream>
#include <span>
#include <stdexcept>

namespace binfo {

BinaryFile::BinaryFile(const std::filesystem::path& p)
    : path(p)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Unable to open file");
    }

    size = static_cast<std::uintmax_t>(file.tellg());

    file.seekg(0);
    file.read(reinterpret_cast<char*>(header_.data()), header_.size());

    format = detect_format(
        std::span<const uint8_t>(header_.data(), static_cast<std::size_t>(file.gcount()))
    );
}

void inspect(const BinaryFile& file)
{
    std::cout << "File: " << file.path << "\n";
    std::cout << "Size: " << file.size << " bytes\n";
    std::cout << "Format: " << file.format << "\n";
}

} // namespace binfo
