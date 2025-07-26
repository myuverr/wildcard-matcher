#pragma once

#include <algorithm>
#include <chrono>
#include <string_view>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using memoized recursion.
 */
class MemoSolver {
   public:
    /**
     * @brief Runs and profiles the memoized recursion algorithm.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * extra space used in bytes.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // Create an instance of the solver
        // This encapsulates all the state and context for a single run
        MemoSolver solver(s, p);
        return solver.run();
    }

   private:
    // --- Member variables holding the constant context for a single run ---
    std::string_view s;
    std::string_view p;
    int m;
    int n;
    std::vector<std::vector<int>> memo;
    size_t max_depth;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s The text string view to match.
     * @param p The pattern string view to match against.
     */
    MemoSolver(std::string_view s_in, std::string_view p_in)
        : s(s_in),
          p(p_in),
          m(s_in.length()),
          n(p_in.length()),
          memo(s_in.length() + 1, std::vector<int>(p_in.length() + 1, -1)),
          max_depth(0) {}

    /**
     * @brief [private] Runs the core logic and profiling for the instance.
     * @return A SolverProfile struct.
     */
    SolverProfile run() {
        // 1. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(0, 0, 0);

        // 2. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 3. Calculate extra space overhead
        // Extra space = memoization table size + max recursion stack depth space

        // 3.1 Calculate the size of the memoization table
        std::size_t memo_space =
            (static_cast<std::size_t>(m) + 1) * (static_cast<std::size_t>(n) + 1) * sizeof(int);

        // 3.2 Calculate the maximum space used by the recursion stack
        // Approximate size of each stack frame = 2 index arguments + 1 return address
        std::size_t space_per_frame = sizeof(int) * 2 + sizeof(void*);
        std::size_t stack_space = max_depth * space_per_frame;
        std::size_t total_space_used = memo_space + stack_space;

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), total_space_used};
    }

    /**
     * @brief [private] Checks if the string and pattern match using memoized recursion.
     *
     * It uses member variables for context (s, p, memo) and takes only indices as parameters.
     *
     * @param i The current index in string s.
     * @param j The current index in pattern p.
     * @param depth The current recursion depth for profiling.
     * @return true if the substring of s from index i matches the subpattern of p from index j,
     * false otherwise.
     */
    bool isMatch(int i, int j, size_t depth) {
        // Update the maximum depth for the current execution context
        max_depth = std::max(max_depth, depth);

        // Check the memoization table.
        // If the subproblem is already solved, return the result directly
        if (memo[i][j] != -1) {
            return memo[i][j] == 1;
        }

        bool ans = false;

        // Pattern p is exhausted
        if (j == n) {
            ans = (i == m);
        } else {
            // Check if the current characters match
            bool first_match = (i < m && (p[j] == '?' || p[j] == s[i]));

            if (p[j] == '*') {
                // If p[j] is '*':
                // Branch 1: '*' matches an empty sequence
                // Branch 2: '*' matches s[i]
                ans = isMatch(i, j + 1, depth + 1) || (i < m && isMatch(i + 1, j, depth + 1));
            } else {
                // If p[j] is a regular character or '?'
                ans = first_match && isMatch(i + 1, j + 1, depth + 1);
            }
        }

        // Store the result in the memoization table before returning
        memo[i][j] = ans ? 1 : 0;
        return ans;
    }
};