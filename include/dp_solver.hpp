#pragma once

#include <cstring>
#include <vector>

/**
 * @brief 动态规划求解器
 */
struct DpSolver {
    /**
     * @brief 使用动态规划检查字符串 s 和模式串 p 是否匹配.
     *
     * 构建一个 dp table, dp[i][j] 表示 s 的前 i 个字符
     * 是否能与 p 的前 j 个字符匹配.
     */
    static bool isMatch(const char* s, const char* p) {
        int m = strlen(s);
        int n = strlen(p);

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

        // 填充 dp table
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                // case: p[j-1] is '*'
                if (p[j - 1] == '*') {
                    // '*' 匹配空串 (等价于 p 截断一位)
                    // 或 '*' 匹配 s[i-1] (等价于 s 截断一位)
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