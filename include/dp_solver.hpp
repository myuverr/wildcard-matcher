#pragma once

#include <chrono>
#include <string_view>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using dynamic programming.
 */
class DpSolver {
   public:
    /**
     * @brief Runs and profiles the dynamic programming algorithm.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * extra space used in bytes.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // 1. Preparation: Calculate lengths
        int m = s.length();
        int n = p.length();

        // 2. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p, m, n);

        // 3. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 4. Calculate extra space overhead
        // The extra space is mainly from the (m+1)x(n+1) DP table.
        std::size_t space_used =
            (static_cast<std::size_t>(m) + 1) * (static_cast<std::size_t>(n) + 1) * sizeof(bool);

        // 5. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

   private:
    /**
     * @brief [private] Checks if the string and pattern match using dynamic programming.
     *
     * This method builds a DP table where dp[i][j] indicates whether the first i
     * characters of s match the first j characters of p. This is the core
     * implementation of the algorithm.
     *
     * @param s The text string view to match.
     * @param p The pattern string view with wildcards.
     * @param m The length of string s.
     * @param n The length of pattern p.
     * @return true if s and p match completely, false otherwise.
     */
    static bool isMatch(std::string_view s, std::string_view p, int m, int n) {
        // dp[i][j]: true if the first i chars of s match the first j chars of p
        std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));

        // An empty pattern matches an empty string.
        dp[0][0] = true;

        // When s is empty, p can only match if it consists of only '*' characters.
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 1];
            }
        }

        // Fill the DP table from the bottom up.
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                // If p[j-1] is '*'
                if (p[j - 1] == '*') {
                    // '*' matches an empty sequence (equivalent to p being one char shorter, i.e.,
                    // dp[i][j-1]) or '*' matches s[i-1] (equivalent to s being one char shorter,
                    // i.e., dp[i-1][j]).
                    dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
                }
                // If p[j-1] is '?' or matches s[i-1]
                else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    // The result depends on the previous subproblem.
                    dp[i][j] = dp[i - 1][j - 1];
                }
                // Character mismatch, dp[i][j] remains false.
            }
        }

        return dp[m][n];
    }
};