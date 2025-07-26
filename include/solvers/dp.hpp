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
        // Create an instance of the solver
        // This encapsulates all the state and context for a single run
        DpSolver solver(s, p);
        return solver.run();
    }

   private:
    // --- Member variables holding the constant context for a single run ---
    std::string_view s;
    std::string_view p;
    int m;
    int n;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s_in The text string view to match.
     * @param p_in The pattern string view to match against.
     */
    DpSolver(std::string_view s_in, std::string_view p_in)
        : s(s_in), p(p_in), m(s_in.length()), n(p_in.length()) {}

    /**
     * @brief [private] Runs the core logic and profiling for the instance.
     * @return A SolverProfile struct.
     */
    SolverProfile run() {
        // 1. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch();

        // 2. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 3. Calculate extra space overhead
        // The extra space is mainly from the (m+1)x(n+1) DP table
        std::size_t space_used =
            (static_cast<std::size_t>(m) + 1) * (static_cast<std::size_t>(n) + 1) * sizeof(bool);

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

    /**
     * @brief [private] Checks if the string and pattern match using dynamic programming.
     *
     * This method builds a DP table where dp[i][j] indicates whether the first i
     * characters of s match the first j characters of p. It uses member variables for context.
     *
     * @return true if s and p match completely, false otherwise.
     */
    bool isMatch() {
        // dp[i][j]: true if the first i chars of s match the first j chars of p
        std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));

        // An empty pattern matches an empty string
        dp[0][0] = true;

        // When s is empty, p can only match if it consists of only '*' characters
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 1];
            }
        }

        // Fill the DP table from the bottom up
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                // If p[j-1] is '*'
                if (p[j - 1] == '*') {
                    // '*' matches an empty sequence (equivalent to p being one char shorter, i.e.,
                    // dp[i][j-1]) or '*' matches s[i-1] (equivalent to s being one char shorter,
                    // i.e., dp[i-1][j])
                    dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
                }
                // If p[j-1] is '?' or matches s[i-1]
                else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    // The result depends on the previous subproblem
                    dp[i][j] = dp[i - 1][j - 1];
                }
                // Character mismatch, dp[i][j] remains false
            }
        }

        return dp[m][n];
    }
};