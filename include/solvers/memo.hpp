#pragma once

#include <algorithm>
#include <chrono>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

#include "utils/compiler.hpp"
#include "utils/parser.hpp"
#include "wildcard_matcher.hpp"

/**
 * @brief Implements the wildcard matching algorithm using memoized recursion.
 */
class MemoSolver {
   public:
    /**
     * @brief Runs and profiles the memoized algorithm using a raw pattern string.
     * @param s The text string view to match.
     * @param p The pattern string view containing wildcards ('?', '*'), literals, and escape
     * sequences.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, std::string_view p) {
        // Parse the raw pattern string into a sequence of tokens.
        auto tokens = Parser::parse(p).tokens;
        return runAndProfile(s, tokens);
    }

    /**
     * @brief Runs and profiles the memoized algorithm using a pre-parsed token vector.
     * @param s The text string view to match.
     * @param p_tokens The tokenized pattern vector.
     * @return A SolverProfile struct containing the match result, time elapsed, and space used.
     */
    static SolverProfile runAndProfile(std::string_view s, const std::vector<Token>& p_tokens) {
        // Create an instance of the solver with the string and tokenized pattern.
        MemoSolver solver(s, p_tokens);
        return solver.run();
    }

   private:
    // --- Member variables holding the context for a single run ---
    const std::string_view s;
    const std::vector<Token>& p_tokens;
    const size_t m;
    const size_t n;
    mutable std::vector<std::vector<std::optional<bool>>> memo;
    mutable size_t max_depth;

    /**
     * @brief [private] Constructor to initialize the solver's context.
     * @param s_in The text string view to match.
     * @param p_tokens_in The vector of tokens representing the pattern.
     */
    MemoSolver(std::string_view s_in, const std::vector<Token>& p_tokens_in)
        : s(s_in),
          p_tokens(p_tokens_in),
          m(s_in.length()),
          n(p_tokens_in.size()),
          memo(s_in.length() + 1,
               std::vector<std::optional<bool>>(p_tokens_in.size() + 1, std::nullopt)),
          max_depth(0) {}

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

        // 3. Calculate extra space overhead
        // Extra space = memoization table size + max recursion stack depth space

        // 3.1 Calculate the size of the memoization table
        std::size_t memo_space = (m + 1) * (n + 1) * sizeof(decltype(memo[0][0]));

        // 3.2 Calculate the maximum space used by the recursion stack
        // Each stack frame is estimated to contain: 2 size_t args + 1 return address
        std::size_t space_per_frame = sizeof(size_t) * 2 + sizeof(void*);
        std::size_t stack_space = max_depth * space_per_frame;
        std::size_t total_space_used = memo_space + stack_space;

        // 4. Return the struct containing the result and profiling data
        return {result, duration.count(), total_space_used};
    }

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
    bool isMatch(size_t i, size_t j, size_t depth) const {
        // Update the maximum recursion depth reached for profiling purposes
        max_depth = std::max(max_depth, depth);

        // If the subproblem is already solved, return the cached result
        if (memo[i][j].has_value()) {
            return *memo[i][j];
        }

        bool ans = false;

        // Base case: If the pattern is exhausted, the match is successful only
        // if the string is also exhausted
        if (j == n) {
            ans = (i == m);
        } else {
            ans = std::visit(
                [&](const auto& token) {
                    using T = std::decay_t<decltype(token)>;
                    if constexpr (std::is_same_v<T, AnySequence>) {
                        // Branch 1: The '*' matches an empty sequence. Skip the '*' token.
                        // Branch 2: The '*' matches one character. Consume a character from the
                        // string and stay at the same '*' token
                        return isMatch(i, j + 1, depth + 1) ||
                               (i < m && isMatch(i + 1, j, depth + 1));
                    } else if constexpr (std::is_same_v<T, AnyChar>) {
                        // If the string is not exhausted, this token matches the current character
                        return i < m && isMatch(i + 1, j + 1, depth + 1);
                    } else if constexpr (std::is_same_v<T, LiteralSequence>) {
                        const std::string& literal = token.value;
                        const size_t literal_len = literal.length();

                        // Check if the string has enough characters remaining to match the literal
                        // and if the substring matches
                        if (i + literal_len <= m && s.compare(i, literal_len, literal) == 0) {
                            // If it matches, continue matching from the end of the literal sequence
                            return isMatch(i + literal_len, j + 1, depth + 1);
                        } else {
                            // The literal does not match
                            return false;
                        }
                    }
                    APP_UNREACHABLE();  // Should not be reached
                },
                p_tokens[j]);
        }

        // Store the result in the memoization table before returning
        return *(memo[i][j] = ans);
    }
};