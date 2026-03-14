#include "detect.hpp"

#include <array>
#include <cstdint>
#include <fstream>

namespace binfo {

std::string detect_format(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return "UNKNOWN";
    }

    std::array<uint8_t, 4> magic{};
    file.read(reinterpret_cast<char*>(magic.data()), magic.size());

    if (file.gcount() < 4) {
        return "UNKNOWN";
    }

    // ELF: 0x7F 'E' 'L' 'F'
    if (magic[0] == 0x7F && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F') {
        return "ELF";
    }

    // PNG: 0x89 'P' 'N' 'G'
    if (magic[0] == 0x89 && magic[1] == 0x50 && magic[2] == 0x4E && magic[3] == 0x47) {
        return "PNG";
    }

    // ZIP: 'P' 'K' 0x03 0x04
    if (magic[0] == 0x50 && magic[1] == 0x4B && magic[2] == 0x03 && magic[3] == 0x04) {
        return "ZIP";
    }

    // Mach-O 64-bit little-endian (arm64, x86_64)
    if (magic[0] == 0xCF && magic[1] == 0xFA && magic[2] == 0xED && magic[3] == 0xFE) {
        return "Mach-O";
    }

    // Mach-O 32-bit little-endian
    if (magic[0] == 0xCE && magic[1] == 0xFA && magic[2] == 0xED && magic[3] == 0xFE) {
        return "Mach-O";
    }

    // Mach-O 64-bit big-endian
    if (magic[0] == 0xFE && magic[1] == 0xED && magic[2] == 0xFA && magic[3] == 0xCF) {
        return "Mach-O";
    }

    // Mach-O 32-bit big-endian
    if (magic[0] == 0xFE && magic[1] == 0xED && magic[2] == 0xFA && magic[3] == 0xCE) {
        return "Mach-O";
    }

    // Mach-O Universal/Fat binary
    if (magic[0] == 0xCA && magic[1] == 0xFE && magic[2] == 0xBA && magic[3] == 0xBE) {
        return "Mach-O Universal";
    }

    return "UNKNOWN";
}

} // namespace binfo
