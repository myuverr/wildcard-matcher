#pragma once

#include <chrono>
#include <cstring>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief 动态规划求解器
 */
struct DpSolver {
   public:
    /**
     * @brief 运行并评测动态规划算法的性能。
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符 '?' 和 '*' 的模式字符串。
     * @return SolverProfile 包含匹配结果、耗时（微秒）和额外空间（字节）的评测数据。
     */
    static SolverProfile runAndProfile(const char* s, const char* p) {
        // 1. 准备工作：计算长度
        int m = strlen(s);
        int n = strlen(p);

        // 2. 计算额外空间开销
        // 额外空间主要来自 m+1 行、n+1 列的二维 dp 表
        std::size_t space_used =
            (static_cast<std::size_t>(m) + 1) * (static_cast<std::size_t>(n) + 1) * sizeof(bool);

        // 3. 启动计时器并执行核心匹配逻辑
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p, m, n);

        // 4. 结束计时并计算耗时
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 5. 返回包含结果和评测数据的结构体
        return {result, duration.count(), space_used};
    }

   private:
    /**
     * @brief [私有] 使用动态规划检查字符串 s 和模式串 p 是否匹配。
     *
     * 构建一个 dp table, 其中 dp[i][j] 表示 s 的前 i 个字符
     * 是否能与 p 的前 j 个字符匹配。此为核心算法实现。
     *
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符的模式字符串。
     * @param m 字符串 s 的长度。
     * @param n 模式串 p 的长度。
     * @return bool 如果 s 和 p 完全匹配，则返回 true，否则返回 false。
     */
    static bool isMatch(const char* s, const char* p, int m, int n) {
        // dp[i][j]: s 的前 i 个字符能否与 p 的前 j 个字符匹配
        std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));

        // base case: 空 pattern 匹配空 string
        dp[0][0] = true;

        // base case: s 为空时, p 只有在全是 '*' 的情况下才能匹配
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 1];
            }
        }

        // 自底向上填充 dp table
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                // case: p[j-1] is '*'
                if (p[j - 1] == '*') {
                    // '*' 匹配空串 (等价于 p 截断一位, 即 dp[i][j-1])
                    // 或 '*' 匹配 s[i-1] (等价于 s 截断一位, 即 dp[i-1][j])
                    dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
                }
                // case: p[j-1] is '?' or a matching character
                else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    // 结果取决于前面的子问题
                    dp[i][j] = dp[i - 1][j - 1];
                }
                // default: 字符不匹配, dp[i][j] 保持 false
            }
        }

        return dp[m][n];
    }
};