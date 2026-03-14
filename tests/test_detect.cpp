#include <catch2/catch_test_macros.hpp>

#include "detect.hpp"
#include "inspect.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <span>

using namespace binfo;

// ---------------------------------------------------------------------------
// detect_format(span) — pure byte logic, no file I/O
// ---------------------------------------------------------------------------

TEST_CASE("detect_format(span) identifies ELF", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0x7F, 0x45, 0x4C, 0x46};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "ELF");
}

TEST_CASE("detect_format(span) identifies PNG", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0x89, 0x50, 0x4E, 0x47};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "PNG");
}

TEST_CASE("detect_format(span) identifies ZIP", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0x50, 0x4B, 0x03, 0x04};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "ZIP");
}

TEST_CASE("detect_format(span) identifies Mach-O 64-bit LE", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0xCF, 0xFA, 0xED, 0xFE};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "Mach-O");
}

TEST_CASE("detect_format(span) identifies Mach-O 32-bit LE", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0xCE, 0xFA, 0xED, 0xFE};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "Mach-O");
}

TEST_CASE("detect_format(span) identifies Mach-O 64-bit BE", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0xFE, 0xED, 0xFA, 0xCF};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "Mach-O");
}

TEST_CASE("detect_format(span) identifies Mach-O 32-bit BE", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0xFE, 0xED, 0xFA, 0xCE};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "Mach-O");
}

TEST_CASE("detect_format(span) identifies Mach-O Universal", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0xCA, 0xFE, 0xBA, 0xBE};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "Mach-O Universal");
}

TEST_CASE("detect_format(span) returns UNKNOWN for all-zeros", "[detect][span]")
{
    std::array<uint8_t, 4> magic = {0x00, 0x00, 0x00, 0x00};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "UNKNOWN");
}

TEST_CASE("detect_format(span) returns UNKNOWN for buffer shorter than 4 bytes", "[detect][span]")
{
    std::array<uint8_t, 3> magic = {0x7F, 0x45, 0x4C};
    REQUIRE(detect_format(std::span<const uint8_t>(magic)) == "UNKNOWN");
}

TEST_CASE("detect_format(span) returns UNKNOWN for empty buffer", "[detect][span]")
{
    REQUIRE(detect_format(std::span<const uint8_t>{}) == "UNKNOWN");
}

// ---------------------------------------------------------------------------
// detect_format(path) — file I/O path
// ---------------------------------------------------------------------------

TEST_CASE("detect_format(path) identifies PNG from real file", "[detect][path]")
{
    std::filesystem::path png = "/usr/share/httpd/icons/tex.png";
    if (!std::filesystem::exists(png)) {
        SKIP("test PNG not available on this system");
    }
    REQUIRE(detect_format(png) == "PNG");
}

TEST_CASE("detect_format(path) returns UNKNOWN for plain text file", "[detect][path]")
{
    std::filesystem::path hosts = "/etc/hosts";
    if (!std::filesystem::exists(hosts)) {
        SKIP("/etc/hosts not available on this system");
    }
    REQUIRE(detect_format(hosts) == "UNKNOWN");
}

TEST_CASE("detect_format(path) returns UNKNOWN for nonexistent file", "[detect][path]")
{
    REQUIRE(detect_format("/tmp/binfo_no_such_file_xyz.bin") == "UNKNOWN");
}

// ---------------------------------------------------------------------------
// BinaryFile — constructor, fields
// ---------------------------------------------------------------------------

TEST_CASE("BinaryFile populates size correctly", "[BinaryFile]")
{
    std::filesystem::path png = "/usr/share/httpd/icons/tex.png";
    if (!std::filesystem::exists(png)) {
        SKIP("test PNG not available on this system");
    }
    BinaryFile bf{png};
    REQUIRE(bf.size == std::filesystem::file_size(png));
}

TEST_CASE("BinaryFile populates format correctly", "[BinaryFile]")
{
    std::filesystem::path png = "/usr/share/httpd/icons/tex.png";
    if (!std::filesystem::exists(png)) {
        SKIP("test PNG not available on this system");
    }
    BinaryFile bf{png};
    REQUIRE(bf.format == "PNG");
}

TEST_CASE("BinaryFile stores path correctly", "[BinaryFile]")
{
    std::filesystem::path png = "/usr/share/httpd/icons/tex.png";
    if (!std::filesystem::exists(png)) {
        SKIP("test PNG not available on this system");
    }
    BinaryFile bf{png};
    REQUIRE(bf.path == png);
}

TEST_CASE("BinaryFile throws on nonexistent file", "[BinaryFile]")
{
    REQUIRE_THROWS_AS(
        BinaryFile{"/tmp/binfo_no_such_file_xyz.bin"},
        std::runtime_error
    );
}
