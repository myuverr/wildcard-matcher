#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using recursive backtracking.
 */
class RecursiveSolver {
   public:
    /**
     * @brief Runs and profiles the recursive algorithm using a pre-parsed token vector.
     * @param s The text string view to match.
     * @param p_tokens The tokenized pattern vector.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, const std::vector<Token>& p_tokens);

   private:
    // --- Member variables holding the constant context for a single run ---
    const std::string_view s;
    const std::vector<Token>& p_tokens;
    const std::size_t m;
    const std::size_t n;
    mutable std::size_t max_depth;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s_in The text string view to match.
     * @param p_tokens_in The vector of tokens representing the pattern.
     */
    RecursiveSolver(std::string_view s_in, const std::vector<Token>& p_tokens_in);

    /**
     * @brief [private] Runs the core logic and profiling for the instance.
     * @return A SolverProfile struct.
     */
    SolverProfile run() const;

    /**
     * @brief [private] Checks if the string and tokenized pattern match using recursive
     * backtracking.
     *
     * It uses member variables for context and tracks recursion depth for profiling.
     *
     * @param i The current index in the input string `s`.
     * @param j The current index in the token pattern `p_tokens`.
     * @param depth The current recursion depth, used for space profiling.
     * @return true if s[i:] and p_tokens[j:] match completely, false otherwise.
     */
    bool isMatch(std::size_t i, std::size_t j, std::size_t depth) const;
};