#pragma once

#include <chrono>
#include <cstring>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief 基于状态机的求解器
 */
struct NFASolver {
   public:
    /**
     * @brief 运行并评测基于状态机的算法的性能。
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符 '?' 和 '*' 的模式字符串。
     * @return SolverProfile 包含匹配结果、耗时（微秒）和额外空间（字节）的评测数据。
     */
    static SolverProfile runAndProfile(const char* s, const char* p) {
        // 1. 准备工作：计算长度
        int m = strlen(s);
        int n = strlen(p);

        // 2. 计算额外空间开销
        // 额外空间主要来自大小为 n+1 的一维 dp 表
        std::size_t space_used = (static_cast<std::size_t>(n) + 1) * sizeof(bool);

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
     * @brief [私有] 使用一维DP（状态机模拟）检查字符串 s 和模式串 p 是否匹配。
     *
     * dp[j] 表示当前 s 字符处理完后, p 的前 j 个字符的匹配状态。
     * 每次迭代 s, 都会更新整个 dp 数组来表示新的状态集。此为核心算法实现。
     *
     * @param s 待匹配的文本字符串。
     * @param p 包含通配符的模式字符串。
     * @param m 字符串 s 的长度。
     * @param n 模式串 p 的长度。
     * @return bool 如果 s 和 p 完全匹配，则返回 true，否则返回 false。
     */
    static bool isMatch(const char* s, const char* p, int m, int n) {
        // dp[j] 对应于二维 DP 中的 dp[i][j]
        std::vector<bool> dp(n + 1, false);

        // base case: 空 string vs 空 pattern
        dp[0] = true;

        // base case: 空 string vs 非空 pattern, 只有当 p 前缀全是 '*' 才可能匹配
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[j] = dp[j - 1];
            }
        }

        // 逐个处理 s 中的字符, 更新 dp 状态
        for (int i = 1; i <= m; ++i) {
            // pre 保存迭代前 dp[j-1] 的状态, 相当于二维DP中的 dp[i-1][j-1]
            bool pre = dp[0];
            // dp[i][0] 恒为 false (非空 string vs 空 pattern)
            dp[0] = false;

            for (int j = 1; j <= n; ++j) {
                bool temp = dp[j];  // 临时保存 dp[j] 的状态, 相当于 dp[i-1][j]

                if (p[j - 1] == '*') {
                    // 状态转移: '*' 匹配空串 (dp[j-1], 即左边状态) 或匹配 s[i-1] (dp[j],
                    // 即上方状态)
                    dp[j] = dp[j - 1] || dp[j];
                } else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    // 状态转移: 依赖于左上角 pre, 即 dp[i-1][j-1]
                    dp[j] = pre;
                } else {
                    // 无法匹配, 状态中断
                    dp[j] = false;
                }
                // 为下个循环更新 pre (原 dp[i-1][j] 成为下一个的 dp[i-1][j-1])
                pre = temp;
            }
        }

        return dp[n];
    }
};