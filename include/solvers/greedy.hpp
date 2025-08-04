#pragma once

#include <chrono>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

#include "utils/parser.hpp"
#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using a two-pointer greedy approach.
 * This solver is now capable of handling tokenized patterns, including literal sequences.
 */
class GreedySolver {
   public:
    /**
     * @brief Runs and profiles the greedy algorithm using a raw pattern string.
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
     * @brief Runs and profiles the greedy algorithm using a pre-parsed token vector.
     * @param s The text string view to match.
     * @param p_tokens The tokenized pattern vector.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, const std::vector<Token>& p_tokens) {
        // Create an instance of the solver with the string and tokenized pattern
        GreedySolver solver(s, p_tokens);
        return solver.run();
    }

   private:
    /**
     * @brief A struct to atomically hold the entire state needed for backtracking.
     */
    struct BacktrackPoint {
        size_t star_p_idx;   // The index of the ANY_SEQUENCE ('*') token in the pattern
        size_t s_match_idx;  // The index in the string 's' where the '*' match began
    };

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
    GreedySolver(std::string_view s_in, const std::vector<Token>& p_tokens_in)
        : s(s_in), p_tokens(p_tokens_in), m(s_in.length()), n(p_tokens_in.size()) {}

    /**
     * @brief [private] Runs the core logic and profiling for the instance.
     * @return A SolverProfile struct.
     */
    SolverProfile run() const {
        // 1. Start the timer and execute the core matching logic
        auto start_time = std::chrono::high_resolution_clock::now();
        bool result = isMatch();

        // 2. Stop the timer and calculate the duration
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // 3. Calculate extra space overhead
        // The extra space is from the two main indices and the optional backtrack state object.
        std::size_t space_used = sizeof(size_t) * 2 + sizeof(std::optional<BacktrackPoint>);

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), space_used};
    }

    /**
     * @brief [private] Checks if the string and tokenized pattern match using a two-pointer greedy
     * approach.
     * @return true if `s` and `p_tokens` match completely, false otherwise.
     */
    bool isMatch() const {
        size_t s_idx = 0;
        size_t p_idx = 0;
        std::optional<BacktrackPoint> backtrack_point;

        while (s_idx < m) {
            // Case 1: A direct match is found (AnyChar or a matching LiteralSequence)
            if (p_idx < n) {
                bool matched = std::visit(
                    [&](const auto& token) {
                        using T = std::decay_t<decltype(token)>;
                        if constexpr (std::is_same_v<T, AnyChar>) {
                            s_idx++;
                            p_idx++;
                            return true;
                        } else if constexpr (std::is_same_v<T, LiteralSequence>) {
                            const std::string& literal = token.value;
                            const size_t literal_len = literal.length();
                            if (m - s_idx >= literal_len &&
                                s.compare(s_idx, literal_len, literal) == 0) {
                                s_idx += literal_len;
                                p_idx++;
                                return true;
                            }
                        }
                        return false;  // No match for this type or condition failed
                    },
                    p_tokens[p_idx]);

                if (matched) {
                    continue;
                }
            }

            // Case 2: If a direct match fails, check for an AnySequence ('*') token
            // Record its position and the current string index as an atomic backtrack point
            if (p_idx < n && std::holds_alternative<AnySequence>(p_tokens[p_idx])) {
                backtrack_point = {p_idx, s_idx};
                p_idx++;
            }
            // Case 3: A mismatch occurred, but have a recorded backtrack point
            else if (backtrack_point.has_value()) {
                // Restore state from the backtrack point
                p_idx = backtrack_point->star_p_idx + 1;
                // Advance the string match position for '*' and backtrack the main string pointer
                backtrack_point->s_match_idx++;
                s_idx = backtrack_point->s_match_idx;
            }
            // Case 4: A definitive mismatch with no option to backtrack
            else {
                return false;
            }
        }

        // The string is exhausted. Consume any trailing ANY_SEQUENCE tokens in the pattern
        while (p_idx < n && std::holds_alternative<AnySequence>(p_tokens[p_idx])) {
            p_idx++;
        }

        // The match is successful only if the pattern is also fully consumed
        return p_idx == n;
    }
};