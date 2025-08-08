#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using dynamic programming.
 */
class DpSolver {
   public:
    /**
     * @brief Runs and profiles the dynamic programming algorithm using a pre-parsed token vector.
     * @param s The text string view to match.
     * @param p_tokens The tokenized pattern vector.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, const std::vector<Token>& p_tokens);

   private:
    /**
     * @brief Represents the state of a DP table entry.
     */
    enum class DpState : std::uint8_t {
        NoMatch = 0,
        Match = 1,
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
    DpSolver(std::string_view s_in, const std::vector<Token>& p_tokens_in);

    /**
     * @brief [private] Runs the core logic and profiling for the instance.
     * @return A SolverProfile struct.
     */
    SolverProfile run();

    /**
     * @brief [private] Checks if the string and tokenized pattern match using dynamic programming.
     *
     * This method builds a DP table where dp[i][j] indicates whether the first i
     * characters of s match the first j tokens of p_tokens.
     *
     * @return true if s and p_tokens match completely, false otherwise.
     */
    bool isMatch();
};