#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using a two-pointer greedy approach.
 * This solver is now capable of handling tokenized patterns, including literal sequences.
 */
class GreedySolver {
   public:
    /**
     * @brief Runs and profiles the greedy algorithm using a pre-parsed token vector.
     * @param s The text string view to match.
     * @param p_tokens The tokenized pattern vector.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, const std::vector<Token>& p_tokens);

   private:
    /**
     * @brief A struct to atomically hold the entire state needed for backtracking.
     */
    struct BacktrackPoint {
        std::size_t star_p_idx;   // The index of the ANY_SEQUENCE ('*') token in the pattern
        std::size_t s_match_idx;  // The index in the string 's' where the '*' match began
    };

    // --- Member variables holding the context for a single run ---
    const std::string_view s;
    const std::vector<Token>& p_tokens;
    const std::size_t m;
    const std::size_t n;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s_in The text string view to match.
     * @param p_tokens_in The vector of tokens representing the pattern.
     */
    GreedySolver(std::string_view s_in, const std::vector<Token>& p_tokens_in);

    /**
     * @brief [private] Runs the core logic and profiling for the instance.
     * @return A SolverProfile struct.
     */
    SolverProfile run() const;

    /**
     * @brief [private] Checks if the string and tokenized pattern match using a two-pointer greedy
     * approach.
     * @return true if `s` and `p_tokens` match completely, false otherwise.
     */
    bool isMatch() const;
};