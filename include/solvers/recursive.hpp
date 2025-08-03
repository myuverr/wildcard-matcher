#pragma once

#include <algorithm>
#include <chrono>
#include <string_view>
#include <vector>

#include "utils/parser.hpp"
#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using recursive backtracking.
 */
class RecursiveSolver {
   public:
    /**
     * @brief Runs and profiles the recursive algorithm using a raw pattern string.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards ('?', '*'), literals, and escape
     * sequences.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // parse the raw pattern string into a sequence of tokens
        auto tokens = Parser::parse(p).tokens;
        return runAndProfile(s, tokens);
    }

    /**
     * @brief Runs and profiles the recursive algorithm using a pre-parsed token vector.
     * @param s The text string view to match.
     * @param p_tokens The tokenized pattern vector.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, const std::vector<Token>& p_tokens) {
        // Create an instance of the solver with the string and tokenized pattern
        RecursiveSolver solver(s, p_tokens);
        return solver.run();
    }

   private:
    // --- Member variables holding the constant context for a single run ---
    const std::string_view s;
    const std::vector<Token>& p_tokens;
    const size_t m;
    const size_t n;
    mutable size_t max_depth;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s_in The text string view to match.
     * @param p_tokens_in The vector of tokens representing the pattern.
     */
    RecursiveSolver(std::string_view s_in, const std::vector<Token>& p_tokens_in)
        : s(s_in), p_tokens(p_tokens_in), m(s_in.length()), n(p_tokens_in.size()), max_depth(0) {}

    /**
     * @brief [private] Runs the core logic and profiling for the instance.
     * @return A SolverProfile struct.
     */
    SolverProfile run() const {
        // 1. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch(0, 0, 0);

        // 2. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 3. Calculate the actual extra space overhead
        // Space overhead = max recursion depth * approximate size of each stack frame
        // Each stack frame is estimated to contain: 2 size_t args + 1 return address
        std::size_t space_per_frame = sizeof(size_t) * 2 + sizeof(void*);
        std::size_t space_used = max_depth * space_per_frame;

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

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
    bool isMatch(size_t i, size_t j, size_t depth) const {
        // Update the maximum recursion depth reached for profiling purposes
        max_depth = std::max(max_depth, depth);

        // Base case: If the pattern is exhausted, the match is successful only if the string is
        // also exhausted
        if (j == n) {
            return i == m;
        }

        const Token& current_token = p_tokens[j];

        switch (current_token.type) {
            case TokenType::ANY_SEQUENCE:  // Corresponds to '*'
                // Branch 1: The '*' matches an empty sequence. Skip the '*' token
                // Branch 2: The '*' matches one or more characters. Consume one character from the
                // string and stay at the same '*' token, which can match more characters
                return isMatch(i, j + 1, depth + 1) || (i < m && isMatch(i + 1, j, depth + 1));

            case TokenType::ANY_CHAR:  // Corresponds to '?'
                // If the string is not exhausted, this token matches the current character
                return i < m && isMatch(i + 1, j + 1, depth + 1);

            case TokenType::LITERAL_SEQUENCE: {
                // This token represents a sequence of one or more literal characters
                const std::string& literal = *current_token.value;
                const size_t literal_len = literal.length();

                // Check if the remaining part of the string is long enough to contain this literal
                // sequence and if the substring actually matches the literal
                if (i + literal_len <= m && s.compare(i, literal_len, literal) == 0) {
                    // If it matches, continue matching from the end of the literal sequence
                    return isMatch(i + literal_len, j + 1, depth + 1);
                }
                break;  // Mismatch if the check fails
            }
        }

        // If none of the above cases resulted in a match, it's a mismatch
        return false;
    }
};