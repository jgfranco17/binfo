#pragma once

#include <cstdint>
#include <filesystem>
#include <span>
#include <string>

namespace binfo {

std::string detect_format(std::span<const uint8_t> header);
std::string detect_format(const std::filesystem::path& path);

} // namespace binfo
