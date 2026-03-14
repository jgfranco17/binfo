# Binfo

Binfo is a command-line binary analysis tool that provides insights into the
structure and behavior of executable files. It is designed to help developers and
security researchers analyze and understand the inner workings of compiled binaries,
making it easier to identify potential vulnerabilities, optimize performance, and
gain a deeper understanding of how software operates at a lower level.

```console
$ binfo /bin/ls
File: "/bin/ls"
Size: 154624 bytes
Format: Mach-O Universal
```

---

## Requirements

- C++20 compiler (`clang++` or `g++`)
- CMake ≥ 3.20 (for tests)
- `clang-tidy` and `clang-format` (for linting/formatting)

---

## Building

```bash
make          # compile → build/binfo
make run      # compile and run (requires a file argument via run target)
make clean    # remove build/
```

The hand-written Makefile compiles with:

```console
-std=c++20 -Wall -Wextra -Wpedantic -g
```

---

## Usage

```bash
./build/binfo <file>
```

Detected formats: `ELF`, `PNG`, `ZIP`, `Mach-O`, `Mach-O Universal`, `UNKNOWN`.

---

## Running Tests

Tests use [Catch2 v3](https://github.com/catchorg/Catch2), fetched automatically via
CMake `FetchContent` on first run — no manual installation required.

```bash
make test
```

This configures a CMake build in `build/cmake/`, builds the test binary, and runs it
via `ctest`.

## Code Quality

```bash
make tidy     # run clang-tidy static analysis
make format   # run clang-format in-place on src/ and include/
```

Active `clang-tidy` check groups:

| Group | Purpose |
| --- | --- |
| `bugprone-*` | Common bug-prone patterns |
| `clang-analyzer-*` | Deep static analysis |
| `modernize-*` | C++11-20 modernization |
| `performance-*` | Unnecessary copies, inefficient constructs |
| `readability-*` | Naming, clarity, redundant code |

`modernize-use-trailing-return-type` is explicitly disabled as a style preference.

---

## Repository Structure

```text
include/
  detect.hpp       # detect_format() declarations
  inspect.hpp      # BinaryFile struct + inspect() declaration

src/
  detect.cpp       # magic-byte format detection (table-driven)
  inspect.cpp      # BinaryFile constructor + inspect() free function
  main.cpp         # CLI entry point

tests/
  test_detect.cpp  # Catch2 test suite

CMakeLists.txt     # CMake build (used for tests)
Makefile           # Hand-written build (used for fast local builds)
```

---

## Development Guidelines

### Code must always compile

Every change must leave the repository in a buildable state. Introduce new modules
header-first, update all `#include` and build files before committing.

### Safe binary parsing

All file reads use `std::ifstream` in binary mode with explicit byte-count checks via
`gcount()`. No raw pointer casts without bounds validation. Prefer `std::span`,
`std::array<uint8_t, N>`, and `std::vector<uint8_t>`.

### Adding a new format

1. Add a `const std::string FORMAT_<NAME>` constant in `src/detect.cpp`
2. Append a row to `kSignatures` in `src/detect.cpp`
3. Add a `TEST_CASE` in `tests/test_detect.cpp` to cover the new magic bytes
4. Run `make && make test && make tidy`

### Module responsibilities

| Module | Responsibility |
| --- | --- |
| `detect.cpp` | Magic-byte detection only — no I/O beyond header reads |
| `inspect.cpp` | `BinaryFile` lifecycle — open file once, populate fields |
| `main.cpp` | CLI argument handling, error reporting |
| `tests/` | Catch2 test cases — no production logic |

### Avoid premature abstraction

Prefer simple structs and free functions. Do not introduce plugin systems, dependency
injection, or template meta-programming until the core functionality justifies it.

### Commit style

Prefer small, focused commits using [Conventional Commits](https://www.conventionalcommits.org/):

- `feat:` — new capability
- `fix:` — bug fix
- `refactor:` — restructuring without behaviour change
- `test:` — test additions or changes
- `chore:` — build/tooling changes
