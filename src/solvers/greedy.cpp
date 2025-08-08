#include "solvers/greedy.hpp"

#include <chrono>
#include <cstddef>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

#include "wildcard_matcher.hpp"

/**
 * @brief Runs and profiles the greedy algorithm using a pre-parsed token vector.
 * @param s The text string view to match.
 * @param p_tokens The tokenized pattern vector.
 * @return A SolverProfile struct containing the match result, time elapsed, and space used.
 */
SolverProfile GreedySolver::runAndProfile(std::string_view s, const std::vector<Token>& p_tokens) {
    // Create an instance of the solver with the string and tokenized pattern
    GreedySolver solver(s, p_tokens);
    return solver.run();
}

/**
 * @brief [private] Constructor to initialize the solver's context.
 * @param s_in The text string view to match.
 * @param p_tokens_in The vector of tokens representing the pattern.
 */
GreedySolver::GreedySolver(std::string_view s_in, const std::vector<Token>& p_tokens_in)
    : s(s_in), p_tokens(p_tokens_in), m(s_in.length()), n(p_tokens_in.size()) {}

/**
 * @brief [private] Runs the core logic and profiling for the instance.
 * @return A SolverProfile struct.
 */
SolverProfile GreedySolver::run() const {
    // 1. Start the timer and execute the core matching logic
    auto start_time = std::chrono::high_resolution_clock::now();
    bool result = isMatch();

    // 2. Stop the timer and calculate the duration
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // 3. Calculate extra space overhead
    // The extra space is from the two main indices and the optional backtrack state object.
    std::size_t space_used = sizeof(std::size_t) * 2 + sizeof(std::optional<BacktrackPoint>);

    // 4. Return the struct containing the result and profiling data
    return {result, duration.count(), space_used};
}

/**
 * @brief [private] Checks if the string and tokenized pattern match using a two-pointer greedy
 * approach.
 * @return true if `s` and `p_tokens` match completely, false otherwise.
 */
bool GreedySolver::isMatch() const {
    std::size_t s_idx = 0;
    std::size_t p_idx = 0;
    std::optional<BacktrackPoint> backtrack_point;

    while (s_idx < m) {
        // Case 1: If there's a pattern token, try to match it
        if (p_idx < n) {
            const auto& current_token = p_tokens[p_idx];
            switch (static_cast<TokenTypeIndex>(current_token.index())) {
                case TokenTypeIndex::AnyChar: {
                    s_idx++;
                    p_idx++;
                    continue;
                }
                case TokenTypeIndex::LiteralSequence: {
                    const auto& literal_seq = std::get<LiteralSequence>(current_token);
                    const std::string& literal = literal_seq.value;
                    const std::size_t literal_len = literal.length();

                    if (m - s_idx >= literal_len && s.compare(s_idx, literal_len, literal) == 0) {
                        s_idx += literal_len;
                        p_idx++;
                        continue;
                    }
                    // Literal does not match, break switch to fall through to backtracking
                    break;
                }
                case TokenTypeIndex::AnySequence: {
                    // Found a '*', this is a potential backtrack point
                    // Record its position and advance pattern pointer
                    backtrack_point = {p_idx, s_idx};
                    p_idx++;
                    continue;
                }
            }
        }

        // Case 2: A mismatch occurred, try to backtrack using the last '*' found
        if (backtrack_point.has_value()) {
            // Restore pattern pointer to the one after the '*'
            p_idx = backtrack_point->star_p_idx + 1;
            // Advance the string match position for '*' and backtrack the main string pointer
            backtrack_point->s_match_idx++;
            s_idx = backtrack_point->s_match_idx;
        }
        // Case 3: A definitive mismatch with no '*' to backtrack to
        else {
            return false;
        }
    }

    // The string is exhausted. Consume any trailing '*' tokens in the pattern
    while (p_idx < n && std::holds_alternative<AnySequence>(p_tokens[p_idx])) {
        p_idx++;
    }

    // The match is successful only if the pattern is also fully consumed
    return p_idx == n;
}