# start

A C++20 library with Python bindings, structured logging, and OpenCV integration.

## Dependencies

| Package | Manager | Version |
|---------|---------|---------|
| opencv4 | Homebrew | system |
| quill | Homebrew | system |
| pybind11 | vcpkg | 2.11.1 |
| gtest | vcpkg | 1.14.0 |

OpenCV and quill are managed via Homebrew (system-wide). pybind11 and gtest are managed via [vcpkg](https://github.com/microsoft/vcpkg) manifest mode.

## Setup

### 1. Install system dependencies via Homebrew (one-time)

```bash
brew install opencv quill
```

### 2. Install vcpkg (one-time)

```bash
git clone https://github.com/microsoft/vcpkg ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
export VCPKG_ROOT=~/vcpkg   # add to ~/.zshrc to persist
```

### 2. Configure and build

vcpkg installs all dependencies automatically during cmake configure.

```bash
cmake --preset vcpkg          # installs deps + configures
cmake --build build --preset release
```

For a debug build:

```bash
cmake --preset vcpkg-debug
cmake --build build --preset debug
```

### Optional features

```bash
# Skip tests
cmake --preset vcpkg -DBUILD_TESTS=OFF

# Build Python bindings
cmake --preset vcpkg -DBUILD_PYTHON=ON
```

## Running tests

```bash
ctest --test-dir build
```
