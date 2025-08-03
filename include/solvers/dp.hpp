#pragma once

#include <chrono>
#include <string_view>
#include <vector>

#include "utils/parser.hpp"
#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using dynamic programming.
 */
class DpSolver {
   public:
    /**
     * @brief Runs and profiles the dynamic programming algorithm using a raw pattern string.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards ('?', '*'), literals, and escape
     * sequences.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // Parse the raw pattern string into a sequence of tokens
        auto tokens = Parser::parse(p).tokens;
        return runAndProfile(s, tokens);
    }

    /**
     * @brief Runs and profiles the dynamic programming algorithm using a pre-parsed token vector.
     * @param s The text string view to match.
     * @param p_tokens The tokenized pattern vector.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, const std::vector<Token>& p_tokens) {
        // Create an instance of the solver with the string and tokenized pattern
        DpSolver solver(s, p_tokens);
        return solver.run();
    }

   private:
    // --- Member variables holding the context for a single run ---
    const std::string_view s;
    const std::vector<Token>& p_tokens;
    const size_t m;
    const size_t n;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s_in The text string view to match.
     * @param p_tokens_in The vector of tokens representing the pattern.
     */
    DpSolver(std::string_view s_in, const std::vector<Token>& p_tokens_in)
        : s(s_in), p_tokens(p_tokens_in), m(s_in.length()), n(p_tokens_in.size()) {}

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

        // 3. Calculate extra space overhead from the (m+1)x(n+1) DP table
        std::size_t space_used = (m + 1) * (n + 1) * sizeof(bool);

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

    /**
     * @brief [private] Checks if the string and tokenized pattern match using dynamic programming.
     *
     * This method builds a DP table where dp[i][j] indicates whether the first i
     * characters of s match the first j tokens of p_tokens.
     *
     * @return true if s and p_tokens match completely, false otherwise.
     */
    bool isMatch() {
        // dp[i][j]: true if the first i chars of s match the first j tokens of p_tokens
        std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));

        // An empty pattern matches an empty string
        dp[0][0] = true;

        // When s is empty, p_tokens can only match if it consists of only '*' tokens
        for (size_t j = 1; j <= n; ++j) {
            if (p_tokens[j - 1].type == TokenType::ANY_SEQUENCE) {
                dp[0][j] = dp[0][j - 1];
            }
        }

        // Fill the DP table
        for (size_t i = 1; i <= m; ++i) {
            for (size_t j = 1; j <= n; ++j) {
                const Token& current_token = p_tokens[j - 1];
                switch (current_token.type) {
                    case TokenType::ANY_SEQUENCE:
                        // The '*' token can either match an empty sequence (dp[i][j-1]) or match
                        // the current character s[i-1] (dp[i-1][j])
                        dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
                        break;

                    case TokenType::ANY_CHAR:
                        // The '?' token matches any single character, so the result depends on the
                        // subproblem without the current character and token
                        dp[i][j] = dp[i - 1][j - 1];
                        break;

                    case TokenType::LITERAL_SEQUENCE: {
                        const std::string& literal = *current_token.value;
                        const size_t literal_len = literal.length();
                        // Check if the string has enough preceding characters and if the substring
                        // ending at s[i-1] matches the literal
                        if (i >= literal_len &&
                            s.compare(i - literal_len, literal_len, literal) == 0) {
                            // If they match, the result depends on the state before this literal
                            // match
                            dp[i][j] = dp[i - literal_len][j - 1];
                        }
                        // else, dp[i][j] remains false
                        break;
                    }
                }
            }
        }

        return dp[m][n];
    }
};