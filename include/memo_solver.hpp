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
   private:
    /**
     * @brief [private] Encapsulates the core state for matching logic.
     * Holds string_views by value and a reference to the shared memo table.
     */
    struct MatchingState {
        std::string_view s;
        std::string_view p;
        int i;
        int j;
        int m;
        int n;
        std::vector<std::vector<int>>& memo;
    };

    /**
     * @brief [private] Encapsulates the state for performance profiling.
     */
    struct ProfilingState {
        size_t depth;
        size_t& max_depth;
    };

   public:
    /**
     * @brief Runs and profiles the memoized recursion algorithm.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * extra space used in bytes.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // 1. Preparation

        // 1.1 Calculate lengths
        int m = s.length();
        int n = p.length();

        // 1.2 Reset depth counters
        size_t max_depth = 0;

        // 1.3 Initialize memoization table
        // -1: not computed, 0: false, 1: true
        std::vector<std::vector<int>> memo(m + 1, std::vector<int>(n + 1, -1));

        // 2. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch({s, p, 0, 0, m, n, memo}, {0, max_depth});

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
        // Approximate size of each stack frame = 2 struct arguments + 1 return address.
        std::size_t space_per_frame =
            sizeof(MatchingState) + sizeof(ProfilingState) + sizeof(void*);
        std::size_t stack_space = max_depth * space_per_frame;
        std::size_t total_space_used = memo_space + stack_space;

        // 5. Return the struct containing the result and profiling data
        return {result, duration.count(), total_space_used};
    }

   private:
    /**
     * @brief [private] Checks if the string and pattern match using memoized recursion.
     *
     * This is the core implementation. It uses a 2D `memo` array to cache the results
     * of subproblems and tracks its state via structs.
     *
     * @param state The current matching state, passed by value (contains reference to memo).
     * @param profile The current profiling state.
     * @return true if the substring of s from index i matches the subpattern of p from index j,
     * false otherwise.
     */
    static bool isMatch(MatchingState state, ProfilingState profile) {
        // Update the maximum depth for the current execution context.
        profile.max_depth = std::max(profile.max_depth, profile.depth);

        // Check the memoization table. If the subproblem is already solved, return the result
        // directly.
        if (state.memo[state.i][state.j] != -1) {
            return state.memo[state.i][state.j] == 1;
        }

        bool ans = false;

        // Pattern p is exhausted.
        if (state.j == state.n) {
            ans = (state.i == state.m);
        } else {
            // Check if the current characters match.
            bool first_match = (state.i < state.m &&
                                (state.p[state.j] == '?' || state.p[state.j] == state.s[state.i]));

            if (state.p[state.j] == '*') {
                // If p[j] is '*':
                // Branch 1: '*' matches an empty sequence.
                // Branch 2: '*' matches s[i].
                ans =
                    isMatch({state.s, state.p, state.i, state.j + 1, state.m, state.n, state.memo},
                            {profile.depth + 1, profile.max_depth}) ||
                    (state.i < state.m &&
                     isMatch({state.s, state.p, state.i + 1, state.j, state.m, state.n, state.memo},
                             {profile.depth + 1, profile.max_depth}));
            } else {
                // If p[j] is a regular character or '?'
                ans = first_match && isMatch({state.s, state.p, state.i + 1, state.j + 1, state.m,
                                              state.n, state.memo},
                                             {profile.depth + 1, profile.max_depth});
            }
        }

        // Store the result in the memoization table before returning.
        state.memo[state.i][state.j] = ans ? 1 : 0;
        return ans;
    }
};