#pragma once

#include <filesystem>
#include <string>

namespace binfo {

std::string detect_format(const std::filesystem::path& path);

} // namespace binfo
