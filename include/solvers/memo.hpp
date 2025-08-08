#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using memoized recursion.
 */
class MemoSolver {
   public:
    /**
     * @brief Runs and profiles the memoized algorithm using a pre-parsed token vector.
     * @param s The text string view to match.
     * @param p_tokens The tokenized pattern vector.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, const std::vector<Token>& p_tokens);

   private:
    /**
     * @brief Represents the state of a memoization table entry.
     */
    enum class MatchState : std::uint8_t {
        Uncomputed,
        Match,
        NoMatch,
    };

    // --- Member variables holding the context for a single run ---
    const std::string_view s;
    const std::vector<Token>& p_tokens;
    const std::size_t m;
    const std::size_t n;
    mutable std::vector<std::vector<MatchState>> memo;
    mutable std::size_t max_depth;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s_in The text string view to match.
     * @param p_tokens_in The vector of tokens representing the pattern.
     */
    MemoSolver(std::string_view s_in, const std::vector<Token>& p_tokens_in);

    /**
     * @brief [private] Runs the core logic and profiling for the instance.
     * @return A SolverProfile struct.
     */
    SolverProfile run() const;

    /**
     * @brief [private] Checks if the string and tokenized pattern match using memoized recursion.
     *
     * It uses member variables for context (s, p_tokens, memo) and takes only indices as
     * parameters.
     *
     * @param i The current index in the input string `s`.
     * @param j The current index in the token pattern `p_tokens`.
     * @param depth The current recursion depth, used for space profiling.
     * @return true if s[i:] and p_tokens[j:] match, false otherwise.
     */
    bool isMatch(std::size_t i, std::size_t j, std::size_t depth) const;
};