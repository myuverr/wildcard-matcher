#pragma once

#include <chrono>

#include "wildcard_matcher.hpp"

/**
 * @brief 递归与回溯求解器
 */
struct RecursiveSolver {
   private:
    // 使用 static 成员在头文件中追踪递归深度
    inline static size_t current_depth = 0;
    inline static size_t max_depth = 0;

    // 辅助类，用于在 isMatch 的作用域内自动管理递归深度
    struct DepthTracker {
        DepthTracker() {
            current_depth++;
            // 更新全局最大深度
            if (current_depth > max_depth) {
                max_depth = current_depth;
            }
        }
        ~DepthTracker() { current_depth--; }
    };

   public:
    /**
     * @brief 运行并评测递归回溯算法的性能。
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符 '?' 和 '*' 的模式字符串。
     * @return SolverProfile 包含匹配结果、耗时（微秒）和实际额外空间（字节）的评测数据。
     */
    static SolverProfile runAndProfile(const char* s, const char* p) {
        // 1. 在每次评测开始前，重置深度计数器
        current_depth = 0;
        max_depth = 0;

        // 2. 启动计时器并执行核心匹配逻辑
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p);

        // 3. 结束计时并计算耗时
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 4. 计算实际的额外空间开销
        // 空间开销 = 实际测得的最大递归深度 * 每个栈帧的大致大小
        // 每个栈帧估算包含：2个指针参数 + 1个返回地址
        std::size_t space_per_frame = sizeof(const char*) * 2 + sizeof(void*);
        std::size_t space_used = max_depth * space_per_frame;

        // 5. 返回包含结果和评测数据的结构体
        return {result, duration.count(), space_used};
    }

   private:
    /**
     * @brief [私有] 使用递归回溯法检查字符串 s 和模式串 p 是否匹配。
     *
     * 此为核心算法实现。通过深度优先搜索探索所有可能的匹配路径。
     *
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符的模式字符串。
     * @return bool 如果 s 和 p 完全匹配，则返回 true，否则返回 false。
     */
    static bool isMatch(const char* s, const char* p) {
        // 在函数入口创建追踪器，在其作用域结束时自动析构
        DepthTracker tracker;

        // base case: p 耗尽, 当 s 也耗尽时匹配成功
        if (*p == '\0') {
            return *s == '\0';
        }

        // case: p 当前字符是 '*'
        if (*p == '*') {
            // 优化: 跳过连续的 '*'
            while (*(p + 1) == '*') {
                p++;
            }

            // 产生两个分支 (depth-first search):
            // 1. '*' 匹配空序列, pattern 后移 (p+1)
            // 2. s 非空, 让 '*' 匹配 s 的一个字符, s 后移 (s+1), pattern 不变
            return isMatch(s, p + 1) || (*s != '\0' && isMatch(s + 1, p));
        }

        // case: p 当前是 '?' 或普通字符
        // s 必须非空，且当前字符需要匹配
        if (*s != '\0' && (*p == '?' || *p == *s)) {
            return isMatch(s + 1, p + 1);
        }

        // 其他情况均不匹配
        return false;
    }
};