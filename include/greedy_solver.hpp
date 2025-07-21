#pragma once

#include <chrono>
#include <string_view>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using a two-pointer greedy approach.
 */
class GreedySolver {
   public:
    /**
     * @brief Runs and profiles the two-pointer greedy algorithm.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * extra space used in bytes.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // Create an instance of the solver
        // This encapsulates all the state and context for a single run
        GreedySolver solver(s, p);
        return solver.run();
    }

   private:
    // --- Member variables holding the constant context for a single run ---
    std::string_view s;
    std::string_view p;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s_in The text string view to match.
     * @param p_in The pattern string view to match against.
     */
    GreedySolver(std::string_view s_in, std::string_view p_in) : s(s_in), p(p_in) {}

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
        // The extra space is from the four indices used for traversal and backtracking
        std::size_t space_used = sizeof(size_t) * 4;  // s_idx, p_idx, star_p_idx, s_match_idx

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

    /**
     * @brief [private] Checks if the string and pattern match using a two-pointer greedy approach.
     *
     * Uses indices s_idx and p_idx for traversal. When a '*' is encountered, its index (star_p_idx)
     * and the corresponding index in s (s_match_idx) are recorded. If a mismatch occurs later,
     * the algorithm backtracks using the recorded indices and resumes matching. It uses member
     * variables for context.
     *
     * @return true if s and p match completely, false otherwise.
     */
    bool isMatch() {
        size_t s_idx = 0;
        size_t p_idx = 0;
        size_t star_p_idx = std::string_view::npos;  // Stores index of last '*' in p
        size_t s_match_idx = 0;                      // Stores index in s to backtrack to

        while (s_idx < s.length()) {
            // Characters match, or p has '?'
            if (p_idx < p.length() && (p[p_idx] == '?' || p[p_idx] == s[s_idx])) {
                s_idx++;
                p_idx++;
            }
            // p has '*', record backtrack position
            else if (p_idx < p.length() && p[p_idx] == '*') {
                star_p_idx = p_idx;
                p_idx++;
                s_match_idx = s_idx;  // Record the matching position in s
            }
            // Mismatch, but there was a '*' before, perform backtracking
            else if (star_p_idx != std::string_view::npos) {
                p_idx = star_p_idx + 1;  // p_idx backtracks to the position after '*'
                s_match_idx++;  // s_match_idx moves forward, meaning '*' matches one more character
                s_idx = s_match_idx;  // s_idx starts from the new s_match_idx position
            }
            // Mismatch, and no '*' to backtrack to
            else {
                return false;
            }
        }

        // s is exhausted, consume any trailing '*' in p
        while (p_idx < p.length() && p[p_idx] == '*') {
            p_idx++;
        }

        // If p is also exhausted, the match is successful
        return p_idx == p.length();
    }
};