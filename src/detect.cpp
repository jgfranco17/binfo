#include "detect.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <span>
#include <string>

namespace binfo {

namespace {

struct FormatSignature {
    std::array<uint8_t, 4> magic;
    std::string name;
};

const std::string FORMAT_ELF       = "ELF";
const std::string FORMAT_PNG       = "PNG";
const std::string FORMAT_ZIP       = "ZIP";
const std::string FORMAT_MACHO     = "Mach-O";
const std::string FORMAT_MACHO_FAT = "Mach-O Universal";
const std::string FORMAT_UNKNOWN   = "UNKNOWN";

const std::array<FormatSignature, 8> kSignatures = {{
    { .magic = {0x7F, 0x45, 0x4C, 0x46}, .name = FORMAT_ELF       }, // ELF
    { .magic = {0x89, 0x50, 0x4E, 0x47}, .name = FORMAT_PNG       }, // PNG
    { .magic = {0x50, 0x4B, 0x03, 0x04}, .name = FORMAT_ZIP       }, // ZIP
    { .magic = {0xCF, 0xFA, 0xED, 0xFE}, .name = FORMAT_MACHO     }, // Mach-O 64-bit LE
    { .magic = {0xCE, 0xFA, 0xED, 0xFE}, .name = FORMAT_MACHO     }, // Mach-O 32-bit LE
    { .magic = {0xFE, 0xED, 0xFA, 0xCF}, .name = FORMAT_MACHO     }, // Mach-O 64-bit BE
    { .magic = {0xFE, 0xED, 0xFA, 0xCE}, .name = FORMAT_MACHO     }, // Mach-O 32-bit BE
    { .magic = {0xCA, 0xFE, 0xBA, 0xBE}, .name = FORMAT_MACHO_FAT }, // Fat binary
}};

} // namespace

std::string detect_format(std::span<const uint8_t> header) {
    if (header.size() < 4) {
        return FORMAT_UNKNOWN;
    }

    for (const auto& sig : kSignatures) {
        if (std::equal(sig.magic.begin(), sig.magic.end(), header.begin())) {
            return sig.name;
        }
    }

    return FORMAT_UNKNOWN;
}

std::string detect_format(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return FORMAT_UNKNOWN;
    }

    std::array<uint8_t, 16> buf{};
    file.read(reinterpret_cast<char*>(buf.data()), buf.size());

    return detect_format(
        std::span<const uint8_t>(buf.data(), static_cast<std::size_t>(file.gcount()))
    );
}

} // namespace binfo
