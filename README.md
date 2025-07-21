# Wildcard Matcher

English | [简体中文](README_zh-Hans.md)

✨ A Comparative C++ Toolkit for Wildcard String Matching ✨

[![C++ Standard](https://img.shields.io/badge/C++-20%2B-blue?style=for-the-badge&logo=cplusplus)](https://isocpp.org/std/status)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-red?style=for-the-badge&logo=cmake)](https://cmake.org)
[![License](https://img.shields.io/github/license/suzato/wildcard-matcher?style=for-the-badge)](LICENSE)

This project provides and benchmarks five distinct algorithms—from recursive backtracking to a space-optimized greedy approach—for solving the classic wildcard matching problem (`?` and `*`).

---

## 📑 Table of Contents

- [✨ Features](#-features)
- [🎯 Problem Definition](#-problem-definition)
- [🚀 Algorithms Implemented](#-algorithms-implemented)
- [⚡ Getting Started](#-getting-started)
- [🛠️ Usage](#️-usage)
- [✅ Testing](#-testing)
- [📜 License](#-license)

## ✨ Features

- **🚀 Five Pluggable Algorithms**: Switch between five distinct solvers, from simple backtracking to a highly optimized greedy approach.
- **📊 Built-in Profiling**: Accurately measure and compare the execution time and memory overhead of each algorithm.
- **🛠️ Modern C++ & CMake**: Built with C++20 and managed with CMake, including automatic dependency handling via `FetchContent`.
- **✅ Well-Tested**: High test coverage using the GoogleTest framework to ensure correctness and robustness.
- **🧩 Cross-Platform**: Compatible with Linux, macOS, and Windows (via MinGW-w64).

## 🎯 Problem Definition

Determine if an input string `s` can be fully matched by a pattern `p`.

- **Input String `s`**: Contains only single-byte ASCII characters (cannot contain `?` or `*`).
- **Pattern `p`**: Contains single-byte ASCII characters, and can include wildcards `?` and `*`.

**Wildcard Rules:**

- `?` matches any single character.
- `*` matches any sequence of characters (including an empty sequence).

## 🚀 Algorithms Implemented

This project provides five strategies to solve the problem. The desired algorithm can be selected at runtime via a command-line flag.

| Algorithm | Time Complexity | Space Complexity | Key Characteristics |
| --------- | --------------- | ---------------- | ------------------- |
| **Recursive Backtracking** | Exponential | `O(m+n)` (stack) | A straightforward, depth-first search approach; inefficient due to re-computation. |
| **Memoized Recursion** | `O(m*n)` | `O(m*n)` | Optimized recursion using a memoization table to cache subproblem results. |
| **Dynamic Programming (2D)** | `O(m*n)` | `O(m*n)` | A robust, bottom-up solution that builds a 2D table. |
| **State Machine (Space-Optimized DP)** | `O(m*n)` | `O(n)` | A space-optimized version of the 2D DP, implemented by simulating an NFA. |
| **Two-Pointer Greedy Algorithm** | `O(m*n)` | `O(1)` | Highly efficient with optimal space, but the logic is intricate and hard to implement. |

<details>
<summary><b>Click to see algorithm details</b></summary>

1. **Recursive Backtracking:** A straightforward, depth-first search approach. It's easy to understand but inefficient due to re-computing overlapping subproblems, leading to exponential time complexity in the worst case.
2. **Memoized Recursion:** An optimization of the recursive approach. It uses a memoization table to cache the results of subproblems, significantly improving performance by reducing the time complexity to polynomial time (`O(m*n)`).
3. **Dynamic Programming (2D):** A bottom-up approach that builds a 2D `dp` table where `dp[i][j]` stores whether the first `i` characters of `s` match the first `j` characters of `p`. It's a standard and robust solution with `O(m*n)` time and space complexity.
4. **State Machine (Space-Optimized DP):** This method simulates a Non-deterministic Finite Automaton (NFA) built from the pattern. It's implemented as a space-optimized version of the 2D DP, reducing the space complexity to `O(n)` while maintaining `O(m*n)` time complexity.
5. **Two-Pointer Greedy Algorithm:** A highly efficient approach using pointers to traverse the strings. It uses a backtracking mechanism with pointers to handle the `*` wildcard. While it achieves an excellent `O(1)` space complexity, the logic is intricate and harder to implement correctly.

</details>

## ⚡ Getting Started

### Prerequisites

- **CMake**: `3.15` or higher
- **C++ Compiler**: C++20 support (e.g., GCC 10+, Clang 10+)
- **(Windows)**: **MinGW-w64** is the recommended toolchain.

### Build Instructions

The project uses CMake's `FetchContent` to manage all dependencies (like GoogleTest) automatically.

```bash
# 1. Clone the repository
git clone https://github.com/suzato/wildcard-matcher.git
cd wildcard-matcher

# 2. Create a build directory
mkdir build
cd build

# 3. Configure with CMake
# On Linux/macOS
cmake ..

# On Windows with MinGW
# cmake -G "MinGW Makefiles" ..

# 4. Compile the project
cmake --build .
```

This will generate two executables in the `build` directory: `wildcard_matcher` and `run_tests`.

## 🛠️ Usage

Execute the main program from within the `build` directory.

```bash
# Run with the default algorithm (Two-Pointer Greedy)
./wildcard_matcher

# Select a specific algorithm (e.g., Dynamic Programming)
./wildcard_matcher --solver dp

# View all available options and solvers
./wildcard_matcher --help
```

The program will then prompt for the input string `s` and pattern `p`, and will output the match result along with performance metrics.

**Example Output:**

```
Result: Match Successful
Performance Metrics:
  - Solver Used: Dynamic Programming
  - Execution Time: ... us
  - Extra Space: ... bytes
```

## ✅ Testing

A comprehensive test suite is included to verify the correctness of all algorithms.

```bash
# Navigate to the build directory
cd build

# Run tests using CTest
ctest --output-on-failure
```

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
