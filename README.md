English | [中文](README_zh.md)

# Wildcard Matcher

[![C++ Standard](https://img.shields.io/badge/C++-20%2B-blue?style=flat&logo=cplusplus)](https://isocpp.org/std/status)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-red?style=flat&logo=cmake)](https://cmake.org)
[![GitHub license](https://img.shields.io/github/license/suzato/wildcard-matcher)](LICENSE)

A C++ project designed to solve the string matching problem with wildcards ('?' and '\*'). This project features five different algorithmic implementations that can be switched via type aliasing in the source code. It also includes a built-in profiling framework to precisely measure and compare the time and space overhead of each algorithm.

## Problem Definition

Determine if an input string `s` can be fully matched by a pattern `p`, subject to the following constraints:

- **Input** `s`: Must contain only single-byte ASCII characters and cannot contain the wildcard characters `?` or `*`.
- **Pattern** `p`: Must contain only single-byte ASCII characters. It can include the wildcards `?` and `*`.

**Wildcard Rules:**

- `?` matches any single character.
- `*` matches any sequence of characters (including an empty sequence).

## Algorithms Implemented

This project provides five distinct strategies to solve the wildcard matching problem.

1.  **Recursive Backtracking:** A straightforward, depth-first search approach. It's easy to understand but inefficient due to re-computing overlapping subproblems, leading to exponential time complexity in the worst case.
2.  **Memoized Recursion:** An optimization of the recursive approach. It uses a memoization table to cache the results of subproblems, significantly improving performance by reducing the time complexity to polynomial time (`O(m*n)`).
3.  **Dynamic Programming (2D):** A bottom-up approach that builds a 2D `dp` table where `dp[i][j]` stores whether the first `i` characters of `s` match the first `j` characters of `p`. It's a standard and robust solution with `O(m*n)` time and space complexity.
4.  **State Machine (Space-Optimized DP):** This method simulates a Non-deterministic Finite Automaton (NFA) built from the pattern. It's implemented as a space-optimized version of the 2D DP, reducing the space complexity to `O(n)` while maintaining `O(m*n)` time complexity.
5.  **Two-Pointer Greedy Algorithm:** A highly efficient approach using pointers to traverse the strings. It uses a backtracking mechanism with pointers to handle the `*` wildcard. While it achieves an excellent `O(1)` space complexity, the logic is intricate and harder to implement correctly.

## Getting Started

### Prerequisites

- CMake (version 3.15 or higher)
- A C++20 compatible compiler (e.g., GCC 10+ or Clang 10+).
- For Windows users, it is recommended to use the **MinGW-w64** toolchain.

### How to Build and Run

**Step 1: Build the Executable**

1. Create and navigate to a build directory:

   ```shell
   mkdir build
   cd build
   ```

2. Configure the project with CMake.

   On Unix-like systems (Linux, macOS), run:

   ```shell
   cmake ..
   ```

   On Windows with MinGW-w64, run:

   ```shell
   cmake -G "MinGW Makefiles" ..
   ```

3. Compile the source code:

   ```shell
   cmake --build .
   ```

   This will create an executable (`wildcard_matcher`) inside the `build` directory.

**Step 2: Execute the Program**

- From within the `build` directory, run the compiled executable.
- You can select an algorithm using the `--solver` or `-s` command-line option.

To run with the default algorithm (Greedy Two-Pointer):

```shell
./wildcard_matcher
```

To select a specific algorithm (e.g., Dynamic Programming):

```shell
./wildcard_matcher --solver dp
```

- The program will then prompt you to enter the text string `s` and the pattern string `p`.

To view all available options and solvers, use the help command:

```shell
./wildcard_matcher --help
```

**Step 3: View the Results**

- The program will output the match result and the performance metrics of the selected algorithm.

Example Output:

```
Result: Match Successful
Performance Metrics:
  - Solver Used: Dynamic Programming
  - Execution Time: ... us
  - Extra Space: ... bytes
```
