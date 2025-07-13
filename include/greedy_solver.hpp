#pragma once

#include <chrono>

#include "wildcard_matcher.hpp"

/**
 * @brief 双指针贪心法算法
 */
class GreedySolver {
   public:
    /**
     * @brief 运行并评测双指针贪心算法的性能。
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符 '?' 和 '*' 的模式字符串。
     * @return SolverProfile 包含匹配结果、耗时（微秒）和额外空间（字节）的评测数据。
     */
    static SolverProfile runAndProfile(const char* s, const char* p) {
        // 1. 准备工作 (此算法无需额外准备)

        // 2. 启动计时器并执行核心匹配逻辑
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p);

        // 3. 结束计时并计算耗时
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 4. 计算额外空间开销
        // 额外空间主要来自4个用于遍历和回溯的指针
        std::size_t space_used = sizeof(const char*) * 4;  // s_ptr, p_ptr, star_ptr, s_match_ptr

        // 5. 返回包含结果和评测数据的结构体
        return {result, duration.count(), space_used};
    }

   private:
    /**
     * @brief [私有] 使用双指针贪心法检查字符串 s 和模式串 p 是否匹配。
     *
     * 使用 s_ptr, p_ptr 遍历。遇到 '*' 时, 记录其位置 (star_ptr)
     * 和对应的 s 的位置 (s_match_ptr)。若后续失配, 则回溯到
     * star_ptr, 并让 s_ptr 从 s_match_ptr 的下一位重新开始。
     *
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符的模式字符串。
     * @return bool 如果 s 和 p 完全匹配，则返回 true，否则返回 false。
     */
    static bool isMatch(const char* s, const char* p) {
        const char* s_ptr = s;
        const char* p_ptr = p;
        const char* star_ptr = nullptr;     // 记录 p 中最后出现的 '*' 指针
        const char* s_match_ptr = nullptr;  // 记录 s 中用于回溯的指针
        while (*s_ptr) {
            // 字符匹配, 或 p 为 '?'
            if (*p_ptr == '?' || *p_ptr == *s_ptr) {
                s_ptr++;
                p_ptr++;
            }
            // p 为 '*', 记录回溯位置
            else if (*p_ptr == '*') {
                star_ptr = p_ptr;
                p_ptr++;
                s_match_ptr = s_ptr;  // 记录 s 的匹配位置
            }
            // 不匹配, 但之前有 '*', 执行回溯
            else if (star_ptr) {
                p_ptr = star_ptr + 1;  // p_ptr 回溯到 '*' 的下一个位置
                s_match_ptr++;         // s_match_ptr 后移, 意味着 '*' 多匹配一个字符
                s_ptr = s_match_ptr;   // s_ptr 从新的 s_match_ptr 位置开始
            }
            // 不匹配, 且无 '*' 可回溯
            else {
                return false;
            }
        }

        // s 已耗尽, 清理 p 尾部的所有 '*'
        while (*p_ptr == '*') {
            p_ptr++;
        }

        // 如果 p 也被耗尽, 则匹配成功
        return !*p_ptr;
    }
};