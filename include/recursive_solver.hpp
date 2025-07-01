#pragma once

/**
 * @brief 递归与回溯求解器
 */
struct RecursiveSolver {
    /**
     * @brief 使用递归回溯法检查字符串 s 和模式串 p 是否匹配.
     */
    static bool isMatch(const char* s, const char* p) {
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