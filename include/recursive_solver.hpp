#pragma once

#include <chrono>
#include <string_view>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using recursive backtracking.
 */
class RecursiveSolver {
   private:
    // Static members for tracking recursion depth.
    inline static size_t current_depth = 0;
    inline static size_t max_depth = 0;

    // Helper struct to automatically manage recursion depth within the scope of isMatch.
    struct DepthTracker {
        DepthTracker() {
            current_depth++;
            // Update the global maximum depth.
            if (current_depth > max_depth) {
                max_depth = current_depth;
            }
        }
        ~DepthTracker() { current_depth--; }
    };

   public:
    /**
     * @brief Runs and profiles the recursive backtracking algorithm.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * actual extra space used in bytes.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // 1. Preparation: Reset depth counters
        current_depth = 0;
        max_depth = 0;

        // 2. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p);

        // 3. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 4. Calculate the actual extra space overhead
        // Space overhead = actual max recursion depth * approximate size of each stack frame.
        // Each stack frame is estimated to contain: 2 string_view arguments + 1 return address.
        std::size_t space_per_frame = sizeof(std::string_view) * 2 + sizeof(void*);
        std::size_t space_used = max_depth * space_per_frame;

        // 5. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

   private:
    /**
     * @brief [private] Checks if string s and pattern p match using recursive backtracking.
     *
     * This is the core implementation. It explores all possible matching paths via
     * depth-first search.
     *
     * @param s The text string view to match.
     * @param p The pattern string view with wildcards.
     * @return true if s and p match completely, false otherwise.
     */
    static bool isMatch(std::string_view s, std::string_view p) {
        // Create a tracker at the function entry to automatically track recursion depth.
        DepthTracker tracker;

        // If p is exhausted, the match is successful only if s is also exhausted.
        if (p.empty()) {
            return s.empty();
        }

        // If the current character in p is '*'
        if (p.front() == '*') {
            // Optimization: skip consecutive '*'
            while (p.length() > 1 && p[1] == '*') {
                p.remove_prefix(1);
            }

            // Create two branches:
            // 1. '*' matches an empty sequence, move pattern forward (p.substr(1)).
            // 2. If s is not empty, let '*' match one character of s, move s forward (s.substr(1)).
            return isMatch(s, p.substr(1)) || (!s.empty() && isMatch(s.substr(1), p));
        }

        // If the current character in p is '?' or a regular character
        // s must not be empty, and the current characters must match.
        if (!s.empty() && (p.front() == '?' || p.front() == s.front())) {
            return isMatch(s.substr(1), p.substr(1));
        }

        // All other cases are a mismatch.
        return false;
    }
};