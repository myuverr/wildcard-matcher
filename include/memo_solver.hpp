#pragma once

#include <chrono>
#include <cstring>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using memoized recursion.
 */
class MemoSolver {
   private:
    // Static members for tracking recursion depth.
    inline static size_t current_depth = 0;
    inline static size_t max_depth = 0;

    // Helper struct to automatically manage recursion depth within the scope of isMatch.
    struct DepthTracker {
        DepthTracker() {
            current_depth++;
            if (current_depth > max_depth) {
                max_depth = current_depth;
            }
        }
        ~DepthTracker() { current_depth--; }
    };

   public:
    /**
     * @brief Runs and profiles the memoized recursion algorithm.
     * @param s The text string to match.
     * @param p The pattern string containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * extra space used in bytes.
     */
    static SolverProfile runAndProfile(const char* s, const char* p) {
        // 1. Preparation

        // 1.1 Calculate lengths
        int m = strlen(s);
        int n = strlen(p);

        // 1.2 Reset depth counters
        current_depth = 0;
        max_depth = 0;

        // 1.3 Initialize memoization table
        // -1: not computed, 0: false, 1: true
        std::vector<std::vector<int>> memo(m + 1, std::vector<int>(n + 1, -1));

        // 2. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p, 0, 0, m, n, memo);

        // 3. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 4. Calculate extra space overhead
        // Extra space = memoization table size + max recursion stack depth space

        // 4.1 Calculate the size of the memoization table
        std::size_t memo_space =
            (static_cast<std::size_t>(m) + 1) * (static_cast<std::size_t>(n) + 1) * sizeof(int);

        // 4.2 Calculate the maximum space used by the recursion stack
        // Approximate size of each stack frame = 5 params (s, p, i, j, memo) + 1 return address
        std::size_t space_per_frame = sizeof(const char*) * 2 + sizeof(int) * 2 +
                                      sizeof(std::vector<std::vector<int>>&) + sizeof(void*);
        std::size_t stack_space = max_depth * space_per_frame;

        std::size_t total_space_used = memo_space + stack_space;

        // 5. Return the struct containing the result and profiling data
        return {result, duration.count(), total_space_used};
    }

   private:
    /**
     * @brief [private] Checks if the string and pattern match using memoized recursion.
     *
     * This is the core implementation of the algorithm. It uses a 2D `memo` array
     * to cache the results of subproblems, avoiding the redundant computations of pure recursion.
     *
     * @param s The original text string.
     * @param p The original pattern string.
     * @param i The current index in s.
     * @param j The current index in p.
     * @param m The total length of string s.
     * @param n The total length of pattern p.
     * @param memo The memoization table for caching results.
     * @return true if the substring of s from index i matches the subpattern of p from index j,
     * false otherwise.
     */
    static bool isMatch(const char* s, const char* p, int i, int j, int m, int n,
                        std::vector<std::vector<int>>& memo) {
        // Create a tracker at the function entry to automatically track recursion depth.
        DepthTracker tracker;

        // Check the memoization table. If the subproblem is already solved, return the result
        // directly.
        if (memo[i][j] != -1) {
            return memo[i][j] == 1;
        }

        bool ans = false;

        // Pattern p is exhausted.
        if (j == n) {
            ans = (i == m);
        } else {
            // Check if the current characters match (i is within bounds and characters are equal,
            // or p[j] is '?').
            bool first_match = (i < m && (p[j] == '?' || p[j] == s[i]));

            if (p[j] == '*') {
                // If p[j] is '*':
                // Branch 1: '*' matches an empty sequence (move pattern pointer forward).
                // Branch 2: '*' matches s[i] (move string pointer forward, pattern pointer stays).
                ans = isMatch(s, p, i, j + 1, m, n, memo) ||
                      (i < m && isMatch(s, p, i + 1, j, m, n, memo));
            } else {
                // If p[j] is a regular character or '?'
                // The current characters must match, and the rest of the strings must also match.
                ans = first_match && isMatch(s, p, i + 1, j + 1, m, n, memo);
            }
        }

        // Store the result in the memoization table before returning.
        memo[i][j] = ans ? 1 : 0;
        return ans;
    }
};