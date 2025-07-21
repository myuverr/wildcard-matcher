#pragma once

#include <chrono>
#include <string_view>
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
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * extra space used in bytes.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // Create an instance of the solver
        // This encapsulates all the state and context for a single run
        NFASolver solver(s, p);
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
    NFASolver(std::string_view s_in, std::string_view p_in)
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
        // The extra space is mainly from the 1D DP table of size n+1
        std::size_t space_used = (static_cast<std::size_t>(n) + 1) * sizeof(bool);

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

    /**
     * @brief [private] Checks if the string and pattern match using 1D DP (state machine
     * simulation).
     *
     * `dp[j]` represents the match status for the first j characters of p after processing
     * the current character of s. With each character from s, the dp array is updated
     * to reflect the new set of states. It uses member variables for context.
     *
     * @return true if s and p match completely, false otherwise.
     */
    bool isMatch() {
        // dp[j] corresponds to dp[i][j] in the 2D DP approach
        std::vector<bool> dp(n + 1, false);

        // An empty pattern matches an empty string
        dp[0] = true;

        // A non-empty pattern can only match an empty string if the prefix is all '*'
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[j] = dp[j - 1];
            }
        }

        // Process characters of s one by one, updating the dp states
        for (int i = 1; i <= m; ++i) {
            // `pre` stores the state of dp[j-1] before the iteration, equivalent to dp[i-1][j-1] in
            // 2D DP
            bool pre = dp[0];
            // dp[i][0] is always false
            dp[0] = false;

            for (int j = 1; j <= n; ++j) {
                bool temp =
                    dp[j];  // Temporarily store the state of dp[j], equivalent to dp[i-1][j]

                if (p[j - 1] == '*') {
                    // State transition: '*' matches an empty string (dp[j-1], the state to the
                    // left) or matches s[i-1] (dp[j], the state from above)
                    dp[j] = dp[j - 1] || dp[j];
                } else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    // State transition: depends on the top-left state `pre`, i.e., dp[i-1][j-1]
                    dp[j] = pre;
                } else {
                    // Mismatch, the state transition is broken
                    dp[j] = false;
                }
                // Update `pre` for the next loop (the old dp[i-1][j] becomes the next
                // dp[i-1][j-1])
                pre = temp;
            }
        }

        return dp[n];
    }
};