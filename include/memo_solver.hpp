#pragma once

#include <chrono>
#include <cstring>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief 带备忘录的递归求解器
 */
struct MemoSolver {
   private:
    // 用于追踪递归深度的静态成员
    inline static size_t current_depth = 0;
    inline static size_t max_depth = 0;

    // 辅助类，用于在 isMatch 的作用域内自动管理递归深度
    struct DepthTracker {
        DepthTracker() {
            current_depth++;
            if (current_depth > max_depth) {
                max_depth = current_depth;
            }
        }
        ~DepthTracker() { current_depth--; }
    };

   public:
    /**
     * @brief 运行并评测带备忘录的递归算法的性能。
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符 '?' 和 '*' 的模式字符串。
     * @return SolverProfile 包含匹配结果、耗时（微秒）和额外空间（字节）的评测数据。
     */
    static SolverProfile runAndProfile(const char* s, const char* p) {
        // 1. 准备工作：计算长度并重置深度计数器
        int m = strlen(s);
        int n = strlen(p);
        current_depth = 0;
        max_depth = 0;

        // 2. 初始化备忘录
        // -1 表示未计算, 0 表示 false, 1 表示 true
        std::vector<std::vector<int>> memo(m + 1, std::vector<int>(n + 1, -1));

        // 3. 启动计时器并执行核心匹配逻辑
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p, 0, 0, m, n, memo);

        // 4. 结束计时并计算耗时
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 5. 计算额外空间开销
        // 额外空间 = 备忘录大小 + 递归栈最大深度占用的空间

        // 5.1 计算备忘录的大小
        std::size_t memo_space =
            (static_cast<std::size_t>(m) + 1) * (static_cast<std::size_t>(n) + 1) * sizeof(int);

        // 5.2 计算递归栈的最大空间
        // 每个递归栈帧的大致大小 = 5个参数(s, p, i, j, memo) + 1个返回地址
        std::size_t space_per_frame = sizeof(const char*) * 2 + sizeof(int) * 2 +
                                      sizeof(std::vector<std::vector<int>>&) + sizeof(void*);
        std::size_t stack_space = max_depth * space_per_frame;

        std::size_t total_space_used = memo_space + stack_space;

        // 6. 返回包含结果和评测数据的结构体
        return {result, duration.count(), total_space_used};
    }

   private:
    /**
     * @brief [私有] 使用带备忘录的递归检查字符串和模式串是否匹配。
     *
     * 这是算法的核心实现。它利用一个二维 `memo` 数组来缓存子问题的解，
     * 避免了纯递归中的大量重复计算。
     *
     * @param s 原始文本字符串。
     * @param p 原始模式字符串。
     * @param i 当前在 s 中的索引。
     * @param j 当前在 p 中的索引。
     * @param m 字符串 s 的总长度。
     * @param n 模式串 p 的总长度。
     * @param memo 用于缓存结果的备忘录。
     * @return bool 如果 s 从索引 i 开始的部分与 p 从索引 j 开始的部分匹配，则返回 true。
     */
    static bool isMatch(const char* s, const char* p, int i, int j, int m, int n,
                        std::vector<std::vector<int>>& memo) {
        // 在函数入口创建追踪器，自动追踪递归深度
        DepthTracker tracker;

        // 检查备忘录，如果子问题已解，直接返回结果
        if (memo[i][j] != -1) {
            return memo[i][j] == 1;
        }

        bool ans = false;

        // Base Case: 模式串 p 已经耗尽
        if (j == n) {
            ans = (i == m);
        } else {
            // 检查当前字符是否匹配（i 未越界且字符相等或 p[j] 为 '?'）
            bool first_match = (i < m && (p[j] == '?' || p[j] == s[i]));

            if (p[j] == '*') {
                // 如果 p[j] 是 '*':
                // 分支1: '*' 匹配空序列 (模式串指针向后移一位)
                // 分支2: '*' 匹配 s[i] (字符串指针向后移一位，模式串指针不变)
                ans = isMatch(s, p, i, j + 1, m, n, memo) ||
                      (i < m && isMatch(s, p, i + 1, j, m, n, memo));
            } else {
                // 如果 p[j] 是普通字符或 '?'
                // 必须当前字符匹配，且剩余部分也要匹配
                ans = first_match && isMatch(s, p, i + 1, j + 1, m, n, memo);
            }
        }

        // 将结果存入备忘录后返回
        memo[i][j] = ans ? 1 : 0;
        return ans;
    }
};