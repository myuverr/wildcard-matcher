# 带通配符的字符串匹配器

[English](README.md) | 简体中文

✨ 一个用于通配符字符串匹配的 C++ 算法对比工具集 ✨

[![C++ Standard](https://img.shields.io/badge/C++-20%2B-blue?style=for-the-badge&logo=cplusplus)](https://isocpp.org/std/status)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-red?style=for-the-badge&logo=cmake)](https://cmake.org)
[![License](https://img.shields.io/github/license/suzato/wildcard-matcher?style=for-the-badge)](LICENSE)

本项目实现并评测了五种不同的算法——从递归回溯到空间优化的贪心策略——用以解决经典的通配符匹配问题（`?` 和 `*`）。

---

## 📑 目录

- [✨ 核心特性](#-核心特性)
- [🎯 问题定义](#-问题定义)
- [🚀 算法实现](#-算法实现)
- [⚡ 快速上手](#-快速上手)
- [🛠️ 如何使用](#️-如何使用)
- [✅ 运行测试](#-运行测试)
- [📜 开源许可](#-开源许可)

## ✨ 核心特性

- **🚀 五种可插拔算法**：从简单的回溯法到高度优化的贪心算法，五种实现可随时切换。
- **📊 内置性能评测**：精确测量并比较每种算法的执行时间与额外空间开销。
- **🛠️ 现代 C++ 与 CMake**：使用 C++20 构建，通过 CMake 和 `FetchContent` 自动管理依赖。
- **✅ 完备的单元测试**：使用 GoogleTest 框架提供高覆盖率的测试，确保算法的正确性和鲁棒性。
- **🧩 跨平台支持**：兼容 Linux、macOS 和 Windows (通过 MinGW-w64)。

## 🎯 问题定义

判断一个输入字符串 `s` 能否被一个模式串 `p` 完全匹配。

- **输入串 `s`**：只包含单字节 ASCII 字符，且不能包含通配符 `?` 或 `*`。
- **模式串 `p`**：包含单字节 ASCII 字符，可以包含通配符 `?` 和 `*`。

**通配符规则:**

- `?` 匹配任意单个字符。
- `*` 匹配任意长度的字符序列（包括空序列）。

## 🚀 算法实现

本项目提供了五种解决该问题的策略。用户可在运行时通过命令行参数选择指定的算法。

| 算法 | 时间复杂度 | 空间复杂度 | 核心特点 |
| ---- | ---------- | ---------- | -------- |
| **递归与回溯** | 指数级 | `O(m+n)` (栈空间) | 逻辑直观的深度优先搜索，但因重叠子问题导致性能极差。 |
| **带备忘录的递归** | `O(m*n)` | `O(m*n)` | 对朴素递归的优化，通过缓存子问题结果避免重复计算。 |
| **动态规划 (2D)** | `O(m*n)` | `O(m*n)` | 自底向上构建二维 `dp` 表，是一种稳健的标准解法。 |
| **基于状态机的实现 (空间优化 DP)** | `O(m*n)` | `O(n)` | 本质是二维 DP 的空间优化版，兼具时间和空间效率。 |
| **双指针贪心法** | `O(m*n)` | `O(1)` | 空间复杂度最优，但逻辑精巧晦涩，是所有方法中最难正确实现的。 |

<details>
<summary><b>点击查看算法详情</b></summary>

1. **递归与回溯 (Recursive Backtracking):** 通过深度优先搜索直接实现。逻辑直观，但因存在大量重叠子问题，未经优化的性能极差，最坏情况下时间复杂度为指数级。
2. **带备忘录的递归 (Memoized Recursion):** 对朴素递归的优化。通过引入备忘录（`memo` 表）缓存子问题的解，避免重复计算，将时间复杂度成功降至多项式级别 (`O(m*n)`)。
3. **动态规划 (Dynamic Programming):** 构建一个二维 `dp` 表，`dp[i][j]` 表示 `s` 的前 `i` 个字符是否能与 `p` 的前 `j` 个字符匹配。这是一种稳健的标准解法，时间与空间复杂度均为 `O(m*n)`。
4. **基于状态机的实现 (空间优化 DP):** 该方法可视为对模式串 `p` 构建的“非确定性有限状态机 (NFA)”的模拟。它本质上是二维 DP 的空间优化版本，将空间复杂度从 `O(m*n)` 降至 `O(n)`，兼具了时间和空间效率。
5. **双指针贪心法 (Two-Pointer Greedy):** 一种空间效率极高的算法。它使用指针进行遍历，并借助额外的回溯指针来处理 `*` 通配符。该算法的空间复杂度达到了最优的 `O(1)`，但其逻辑精巧晦涩，是所有方法中最难正确实现的。

</details>

## ⚡ 快速上手

### 环境要求

- **CMake**: `3.15` 或更高版本
- **C++ 编译器**: 支持 C++20 标准 (例如 GCC 10+, Clang 10+)
- **(Windows)**: 推荐使用 **MinGW-w64** 工具链

### 构建指南

本项目使用 CMake 的 `FetchContent` 模块自动管理所有依赖项（如 GoogleTest）。

```bash
# 1. 克隆仓库
git clone https://github.com/suzato/wildcard-matcher.git
cd wildcard-matcher

# 2. 创建构建目录
mkdir build
cd build

# 3. 使用 CMake 配置项目
# 在 Linux/macOS 上
cmake ..

# 在 Windows 上使用 MinGW
# cmake -G "MinGW Makefiles" ..

# 4. 编译项目
cmake --build .
```

这将在 `build` 目录内生成两个可执行文件：`wildcard_matcher` 和 `run_tests`。

## 🛠️ 如何使用

在 `build` 目录下运行主程序。

```bash
# 使用默认算法（双指针贪心法）运行
./wildcard_matcher

# 选择指定算法（例如：动态规划）运行
./wildcard_matcher --solver dp

# 查看所有可用选项和算法列表
./wildcard_matcher --help
```

程序将提示您输入文本串 `s` 和模式串 `p`，并输出匹配结果以及性能指标。

**输出示例:**

```
Result: Match Successful
Performance Metrics:
  - Solver Used: Dynamic Programming
  - Execution Time: ... us
  - Extra Space: ... bytes
```

## ✅ 运行测试

项目附带一个完备的测试套件，用以确保所有算法的正确性。

```bash
# 进入 build 目录
cd build

# 使用 CTest 运行测试
ctest --output-on-failure
```

## 📜 开源许可

本项目采用 MIT 许可协议。详情请参阅 [LICENSE](LICENSE) 文件。
