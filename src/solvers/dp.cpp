#include "solvers/dp.hpp"

#include <chrono>
#include <cstddef>
#include <string_view>
#include <variant>
#include <vector>

#include "utils/compiler.hpp"
#include "wildcard_matcher.hpp"

/**
 * @brief Runs and profiles the dynamic programming algorithm using a pre-parsed token vector.
 * @param s The text string view to match.
 * @param p_tokens The tokenized pattern vector.
 * @return A SolverProfile struct containing the match result, time elapsed, and space used.
 */
SolverProfile DpSolver::runAndProfile(std::string_view s, const std::vector<Token>& p_tokens) {
    // Create an instance of the solver with the string and tokenized pattern
    DpSolver solver(s, p_tokens);
    return solver.run();
}

/**
 * @brief [private] Constructor to initialize the solver's context.
 * @param s_in The text string view to match.
 * @param p_tokens_in The vector of tokens representing the pattern.
 */
DpSolver::DpSolver(std::string_view s_in, const std::vector<Token>& p_tokens_in)
    : s(s_in), p_tokens(p_tokens_in), m(s_in.length()), n(p_tokens_in.size()) {}

/**
 * @brief [private] Runs the core logic and profiling for the instance.
 * @return A SolverProfile struct.
 */
SolverProfile DpSolver::run() {
    // 1. Start the timer and execute the core matching logic
    auto start_time = std::chrono::high_resolution_clock::now();
    bool result = isMatch();

    // 2. Stop the timer and calculate the duration
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // 3. Calculate extra space overhead from the (m+1)x(n+1) DP table
    std::size_t space_used = (m + 1) * (n + 1) * sizeof(DpState);

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
bool DpSolver::isMatch() {
    // dp[i][j]: true if the first i chars of s match the first j tokens of p_tokens
    std::vector<std::vector<DpState>> dp(m + 1, std::vector<DpState>(n + 1, DpState::NoMatch));

    // An empty pattern matches an empty string
    dp[0][0] = DpState::Match;

    // When s is empty, p_tokens can only match if it consists of only '*' tokens
    for (std::size_t j = 1; j <= n; ++j) {
        if (std::holds_alternative<AnySequence>(p_tokens[j - 1])) {
            dp[0][j] = dp[0][j - 1];
        }
    }

    // Fill the DP table
    for (std::size_t i = 1; i <= m; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            // Use an immediately-invoked lambda to calculate and assign dp[i][j]
            dp[i][j] = [&] {
                const auto& current_token = p_tokens[j - 1];
                switch (static_cast<TokenTypeIndex>(current_token.index())) {
                    case TokenTypeIndex::AnySequence: {
                        // The '*' token can match an empty sequence (dp[i][j-1]) or match the
                        // current char s[i-1] (dp[i-1][j])
                        if (dp[i][j - 1] == DpState::Match || dp[i - 1][j] == DpState::Match) {
                            return DpState::Match;
                        } else {
                            return DpState::NoMatch;
                        }
                    }
                    case TokenTypeIndex::AnyChar: {
                        // The '?' token matches any single character, so the result depends on
                        // the subproblem without the current character and token
                        return dp[i - 1][j - 1];
                    }
                    case TokenTypeIndex::LiteralSequence: {
                        const auto& literal_seq = std::get<LiteralSequence>(current_token);
                        const std::string& literal = literal_seq.value;
                        const std::size_t literal_len = literal.length();
                        // Check if the string has enough preceding characters and if the
                        // substring ending at s[i-1] matches the literal
                        if (i >= literal_len &&
                            s.compare(i - literal_len, literal_len, literal) == 0) {
                            // If they match, the result depends on the state before this
                            // literal match
                            return dp[i - literal_len][j - 1];
                        }
                        // The literal does not match
                        return DpState::NoMatch;
                    }
                }
                APP_UNREACHABLE();  // Should not be reached
            }();  // <-- Immediately invoke the lambda
        }
    }

    return dp[m][n] == DpState::Match;
}