English | [中文](README_zh.md)

# Wildcard Matcher

[![C++ Standard](https://img.shields.io/badge/standard-20%2B-blue?style=flat&logo=cplusplus)](https://isocpp.org/std/status)
[![GitHub license](https://img.shields.io/github/license/suzato/wildcard-matcher)](LICENSE)

A C++ project designed to solve the string matching problem with wildcards ('?' and '\*'). This project features five different algorithmic implementations that can be switched via type aliasing in the source code. It also includes a built-in profiling framework to precisely measure and compare the time and space overhead of each algorithm.

## Problem Definition

Determine if an input string `s` (containing only lowercase letters) can be fully matched by a pattern `p` (containing lowercase letters, '?', and '\*').

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

- A C++20 compatible compiler (e.g., GCC 10+ or Clang 10+).
- For Windows users, it is recommended to use the **MinGW-w64** toolchain.

### How to Run

**Step 1: Select an Algorithm**

- Open the source file `src/main.cpp`.
- Locate the `using` type alias definition.
- Change `SelectedSolver` to the algorithm you want to test.

  ```cpp
  // file: src/main.cpp
  using SelectedSolver = GreedySolver; // <-- Change this line
  ```

**Step 2: Compile the Code**

- Open your terminal and use the following command to compile the program:

  ```bash
  g++ -std=c++20 src/main.cpp -o wildcard_matcher
  ```

  - `-std=c++20` specifies the C++ standard.
  - `-o wildcard_matcher` specifies the output executable name.

**Step 3: Execute the Program**

- Run the compiled executable from your terminal:

  ```bash
  ./wildcard_matcher
  ```

- The program will prompt you to enter the text string `s` and the pattern string `p`.

  ```
  Enter the text string (s): aab
  Enter the pattern string (p): a?b*
  ```

**Step 4: View the Results**

- The program will output the match result and the performance metrics of the selected algorithm.

  ```
  Result: Match Successful
  Performance Metrics:
    - Execution Time: ... μs
    - Extra Space: ... bytes
  ```
