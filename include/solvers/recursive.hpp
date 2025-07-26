#pragma once

#include <algorithm>
#include <chrono>
#include <string_view>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using recursive backtracking.
 */
class RecursiveSolver {
   public:
    /**
     * @brief Runs and profiles the recursive backtracking algorithm.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * actual extra space used in bytes.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // Create an instance of the solver
        // This encapsulates all the state and context for a single run
        RecursiveSolver solver(s, p);
        return solver.run();
    }

   private:
    // --- Member variables holding the constant context for a single run ---
    std::string_view s;
    std::string_view p;
    int m;
    int n;
    size_t max_depth;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s The text string view to match.
     * @param p The pattern string view to match against.
     */
    RecursiveSolver(std::string_view s_in, std::string_view p_in)
        : s(s_in), p(p_in), m(s_in.length()), n(p_in.length()), max_depth(0) {}

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

        // 3. Calculate the actual extra space overhead
        // Space overhead = actual max recursion depth * approximate size of each stack frame
        // Each stack frame is estimated to contain: 2 index arguments + 1 return address
        std::size_t space_per_frame = sizeof(int) * 2 + sizeof(void*);
        std::size_t space_used = max_depth * space_per_frame;

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

    /**
     * @brief [private] Checks if the string and pattern match using recursive backtracing.
     *
     * It uses member variables for context and tracks recursion depth for profiling.
     *
     * @param i The current index in string s.
     * @param j The current index in pattern p.
     * @param depth The current recursion depth.
     * @return true if s[i:] and p[j:] match completely, false otherwise.
     */
    bool isMatch(int i, int j, size_t depth) {
        // Update the maximum depth for the current execution context
        max_depth = std::max(max_depth, depth);

        // If p is exhausted, the match is successful only if s is also exhausted
        if (j == n) {
            return i == m;
        }

        // If the current character in p is '*'
        if (p[j] == '*') {
            // Branch 1: '*' matches an empty sequence (skip '*')
            // Branch 2: '*' matches one character in s (use '*')
            return isMatch(i, j + 1, depth + 1) || (i < m && isMatch(i + 1, j, depth + 1));
        }

        // If the current character in p is '?' or a regular character
        if (i < m && (p[j] == '?' || p[j] == s[i])) {
            return isMatch(i + 1, j + 1, depth + 1);
        }

        // All other cases are a mismatch
        return false;
    }
};