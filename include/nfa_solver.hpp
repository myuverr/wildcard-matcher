#pragma once

#include <chrono>
#include <cstring>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using a state machine approach (space-optimized
 * DP).
 */
class NFASolver {
   public:
    /**
     * @brief Runs and profiles the state machine-based algorithm.
     * @param s The text string to match.
     * @param p The pattern string containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * extra space used in bytes.
     */
    static SolverProfile runAndProfile(const char* s, const char* p) {
        // 1. Preparation: Calculate lengths
        int m = strlen(s);
        int n = strlen(p);

        // 2. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p, m, n);

        // 3. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 4. Calculate extra space overhead
        // The extra space is mainly from the 1D DP table of size n+1.
        std::size_t space_used = (static_cast<std::size_t>(n) + 1) * sizeof(bool);

        // 5. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

   private:
    /**
     * @brief [private] Checks if the string and pattern match using 1D DP (state machine
     * simulation).
     *
     * `dp[j]` represents the match status for the first j characters of p after processing
     * the current character of s. With each character from s, the dp array is updated
     * to reflect the new set of states. This is the core implementation of the algorithm.
     *
     * @param s The text string to match.
     * @param p The pattern string with wildcards.
     * @param m The length of string s.
     * @param n The length of pattern p.
     * @return true if s and p match completely, false otherwise.
     */
    static bool isMatch(const char* s, const char* p, int m, int n) {
        // dp[j] corresponds to dp[i][j] in the 2D DP approach.
        std::vector<bool> dp(n + 1, false);

        // An empty pattern matches an empty string.
        dp[0] = true;

        // A non-empty pattern can only match an empty string if the prefix is all '*'.
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[j] = dp[j - 1];
            }
        }

        // Process characters of s one by one, updating the dp states.
        for (int i = 1; i <= m; ++i) {
            // `pre` stores the state of dp[j-1] before the iteration, equivalent to dp[i-1][j-1] in
            // 2D DP.
            bool pre = dp[0];
            // dp[i][0] is always false.
            dp[0] = false;

            for (int j = 1; j <= n; ++j) {
                bool temp =
                    dp[j];  // Temporarily store the state of dp[j], equivalent to dp[i-1][j].

                if (p[j - 1] == '*') {
                    // State transition: '*' matches an empty string (dp[j-1], the state to the
                    // left) or matches s[i-1] (dp[j], the state from above).
                    dp[j] = dp[j - 1] || dp[j];
                } else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    // State transition: depends on the top-left state `pre`, i.e., dp[i-1][j-1].
                    dp[j] = pre;
                } else {
                    // Mismatch, the state transition is broken.
                    dp[j] = false;
                }
                // Update `pre` for the next loop (the old dp[i-1][j] becomes the next
                // dp[i-1][j-1]).
                pre = temp;
            }
        }

        return dp[n];
    }
};