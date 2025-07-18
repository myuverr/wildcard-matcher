#pragma once

#include <algorithm>
#include <chrono>
#include <string_view>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using recursive backtracking.
 */
class RecursiveSolver {
   private:
    /**
     * @brief [private] Encapsulates the core state required for the matching logic.
     */
    struct MatchingState {
        std::string_view s;
        std::string_view p;
    };

    /**
     * @brief [private] Encapsulates the state required for performance profiling.
     */
    struct ProfilingState {
        size_t depth;
        size_t& max_depth;
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
        size_t max_depth = 0;

        // 2. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch({s, p}, {0, max_depth});

        // 3. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 4. Calculate the actual extra space overhead
        // Space overhead = actual max recursion depth * approximate size of each stack frame.
        // Each stack frame is estimated to contain: 1 struct arguments + 1 return address.
        std::size_t space_per_frame = sizeof(MatchingState) + sizeof(void*);
        std::size_t space_used = max_depth * space_per_frame;

        // 5. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

   private:
    /**
     * @brief [private] The core recursive helper function that is thread-safe.
     *
     * It tracks recursion depth and other states via struct parameters rather than static
     * variables, separating matching logic from profiling.
     *
     * @param state The current matching state (s and p).
     * @param profile The current profiling state (depth and max_depth).
     * @return true if s and p match completely, false otherwise.
     */
    static bool isMatch(MatchingState state, ProfilingState profile) {
        // Update the maximum depth for the current execution context.
        profile.max_depth = std::max(profile.max_depth, profile.depth);

        // If p is exhausted, the match is successful only if s is also exhausted.
        if (state.p.empty()) {
            return state.s.empty();
        }

        // If the current character in p is '*'
        if (state.p.front() == '*') {
            while (state.p.length() > 1 && state.p[1] == '*') {
                state.p.remove_prefix(1);
            }

            // Create two branches:
            return isMatch({state.s, state.p.substr(1)}, {profile.depth + 1, profile.max_depth}) ||
                   (!state.s.empty() &&
                    isMatch({state.s.substr(1), state.p}, {profile.depth + 1, profile.max_depth}));
        }

        // If the current character in p is '?' or a regular character
        if (!state.s.empty() && (state.p.front() == '?' || state.p.front() == state.s.front())) {
            return isMatch({state.s.substr(1), state.p.substr(1)},
                           {profile.depth + 1, profile.max_depth});
        }

        // All other cases are a mismatch.
        return false;
    }
};