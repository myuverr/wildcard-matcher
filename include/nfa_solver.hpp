#pragma once

#include <cstring>
#include <vector>

/**
 * @brief 基于状态机的求解器
 */
struct NFASolver {
    /**
     * @brief 使用一维 DP 模拟 NFA 状态转移来检查匹配.
     *
     * dp[j] 表示当前 s 字符处理完后, p 的前 j 个字符的匹配状态.
     * 每次迭代 s, 都会更新整个 dp 数组来表示新的状态集.
     */
    static bool isMatch(const char* s, const char* p) {
        int m = strlen(s);
        int n = strlen(p);

        // dp[j] 对应于二维 DP 中的 dp[i][j]
        std::vector<bool> dp(n + 1, false);

        // base case: 空 string vs 空 pattern
        dp[0] = true;

        // base case: 空 string vs 非空 pattern
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[j] = dp[j - 1];
            }
        }

        // 逐个处理 s 中的字符, 更新 dp 状态
        for (int i = 1; i <= m; ++i) {
            // pre 保存 dp[i-1][j-1] 的状态
            bool pre = dp[0];
            // dp[i][0] 恒为 false (非空 string vs 空 pattern)
            dp[0] = false;

            for (int j = 1; j <= n; ++j) {
                bool temp = dp[j];  // 临时保存 dp[i-1][j] 的状态

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
                pre = temp;  // 为下个循环更新 pre (原 dp[i-1][j] 成为下一个的 dp[i-1][j-1])
            }
        }

        return dp[n];
    }
};