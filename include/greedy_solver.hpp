#pragma once

#include <chrono>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using a two-pointer greedy approach.
 */
class GreedySolver {
   public:
    /**
     * @brief Runs and profiles the two-pointer greedy algorithm.
     * @param s The text string to match.
     * @param p The pattern string containing wildcards '?' and '*'.
     * @return A SolverProfile struct containing the match result, time elapsed in microseconds, and
     * extra space used in bytes.
     */
    static SolverProfile runAndProfile(const char* s, const char* p) {
        // 1. Preparation (no extra setup needed for this algorithm)

        // 2. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(s, p);

        // 3. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 4. Calculate extra space overhead
        // The extra space is mainly from the four pointers used for traversal and backtracking.
        std::size_t space_used = sizeof(const char*) * 4;  // s_ptr, p_ptr, star_ptr, s_match_ptr

        // 5. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

   private:
    /**
     * @brief [private] Checks if the string and pattern match using a two-pointer greedy approach.
     *
     * Uses s_ptr and p_ptr for traversal. When a '*' is encountered, its position (star_ptr)
     * and the corresponding position in s (s_match_ptr) are recorded. If a mismatch occurs later,
     * the algorithm backtracks to the star_ptr and resumes matching s_ptr from the next
     * position of s_match_ptr.
     *
     * @param s The text string to match.
     * @param p The pattern string with wildcards.
     * @return true if s and p match completely, false otherwise.
     */
    static bool isMatch(const char* s, const char* p) {
        const char* s_ptr = s;
        const char* p_ptr = p;
        const char* star_ptr = nullptr;     // Pointer to the last '*' in p
        const char* s_match_ptr = nullptr;  // Pointer in s to backtrack to
        while (*s_ptr) {
            // Characters match, or p has '?'
            if (*p_ptr == '?' || *p_ptr == *s_ptr) {
                s_ptr++;
                p_ptr++;
            }
            // p has '*', record backtrack position
            else if (*p_ptr == '*') {
                star_ptr = p_ptr;
                p_ptr++;
                s_match_ptr = s_ptr;  // Record the matching position in s
            }
            // Mismatch, but there was a '*' before, perform backtracking
            else if (star_ptr) {
                p_ptr = star_ptr + 1;  // p_ptr backtracks to the position after '*'
                s_match_ptr++;  // s_match_ptr moves forward, meaning '*' matches one more character
                s_ptr = s_match_ptr;  // s_ptr starts from the new s_match_ptr position
            }
            // Mismatch, and no '*' to backtrack to
            else {
                return false;
            }
        }

        // s is exhausted, consume any trailing '*' in p
        while (*p_ptr == '*') {
            p_ptr++;
        }

        // If p is also exhausted, the match is successful
        return !*p_ptr;
    }
};