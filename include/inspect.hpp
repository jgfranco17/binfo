#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>

namespace binfo {

struct BinaryFile {
    std::filesystem::path path;
    std::uintmax_t size{};
    std::string format;

    explicit BinaryFile(const std::filesystem::path& p);

private:
    std::array<uint8_t, 16> header_{};
};

void inspect(const BinaryFile& file);

} // namespace binfo
