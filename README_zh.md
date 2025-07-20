[English](README.md) | 中文

# 带通配符的字符串匹配器

[![C++ Standard](https://img.shields.io/badge/C++-20%2B-blue?style=flat&logo=cplusplus)](https://isocpp.org/std/status)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-red?style=flat&logo=cmake)](https://cmake.org)
[![GitHub license](https://img.shields.io/github/license/suzato/wildcard-matcher)](LICENSE)

一个 C++ 项目，用于解决带通配符 `?` 和 `*` 的字符串匹配问题。项目内置了五种不同的算法实现，可通过修改源代码中的类型别名进行切换。项目还内置了一套性能评测框架，可以精确测量并比较不同算法在时间与空间上的开销。

## 问题定义

判断一个字符串 `s` 能否被一个模式串 `p` 完全匹配，需遵循以下约束：

- **输入串** `s`: 必须只包含单字节 ASCII 字符，且不能包含通配符 `?` 或 `*`。
- **模式串** `p`: 必须只包含单字节 ASCII 字符，可以包含通配符 `?` 和 `*`。

**通配符规则：**

- `?` 匹配任意单个字符。
- `*` 匹配任意长度的字符串序列（包括空序列）。

## 算法实现

本项目提供了五种解决该问题的策略。

1.  **递归与回溯 (Recursive Backtracking):** 通过深度优先搜索直接实现。逻辑直观，但因存在大量重叠子问题，未经优化的性能极差，最坏情况下时间复杂度为指数级。
2.  **带备忘录的递归 (Memoized Recursion):** 对朴素递归的优化。通过引入备忘录（`memo` 表）缓存子问题的解，避免重复计算，将时间复杂度成功降至多项式级别 (`O(m*n)`)。
3.  **动态规划 (Dynamic Programming):** 构建一个二维 `dp` 表，`dp[i][j]` 表示 `s` 的前 `i` 个字符是否能与 `p` 的前 `j` 个字符匹配。这是一种稳健的标准解法，时间与空间复杂度均为 `O(m*n)`。
4.  **基于状态机的实现 (空间优化 DP):** 该方法可视为对模式串 `p` 构建的“非确定性有限状态机 (NFA)”的模拟。它本质上是二维 DP 的空间优化版本，将空间复杂度从 `O(m*n)` 降至 `O(n)`，兼具了时间和空间效率。
5.  **双指针贪心法 (Two-Pointer Greedy):** 一种空间效率极高的算法。它使用指针进行遍历，并借助额外的回溯指针来处理 `*` 通配符。该算法的空间复杂度达到了最优的 `O(1)`，但其逻辑精巧晦涩，是所有方法中最难正确实现的。

## 开始使用

### 环境要求

- CMake (版本 3.15 或更高)
- 支持 C++20 标准的编译器 (例如 GCC 10+ 或 Clang 10+)。
- Windows 用户推荐使用 **MinGW-w64** 工具链。

### 如何构建与运行

**步骤 1：构建可执行文件**

1. 创建并进入一个构建目录：

   ```shell
   mkdir build
   cd build
   ```

2. 使用 CMake 配置项目。

   在类 Unix 系统 (Linux, macOS) 上，运行：

   ```shell
   cmake ..
   ```

   在 Windows 上使用 MinGW-w64，运行：

   ```shell
   cmake -G "MinGW Makefiles" ..
   ```

3. 编译源代码：

   ```shell
   cmake --build .
   ```

   这将在 `build` 目录内生成一个可执行文件 (`wildcard_matcher`)。

**步骤 2：执行程序**

- 在 `build` 目录下，运行编译好的可执行文件。
- 你可以通过 `--solver` 或 `-s` 命令行选项来选择算法。

使用默认算法（双指针贪心法）运行：

```shell
./wildcard_matcher
```

选择指定算法（例如：动态规划）运行：

```shell
./wildcard_matcher --solver dp
```

- 程序随后将提示您依次输入文本字符串 `s` 和模式字符串 `p`。

要查看所有可用选项和算法列表，请使用帮助命令：

```shell
./wildcard_matcher --help
```

**步骤 3：查看结果**

- 程序会输出匹配结果以及所选算法的性能指标。

示例输出：

```
Result: Match Successful
Performance Metrics:
  - Solver Used: Dynamic Programming
  - Execution Time: ... us
  - Extra Space: ... bytes
```
