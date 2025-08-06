#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "utils/issues.hpp"
#include "utils/token.hpp"

/**
 * @brief Raw information about a potential issue discovered during parsing.
 */
struct ParseEvent {
    IssueCode code;
    size_t position;                    // 1-based position of the event in the raw pattern string.
    std::optional<std::string> detail;  // e.g., the specific character in an escape sequence.
    bool operator==(const ParseEvent& other) const = default;
};

/**
 * @brief Holds the result of a parsing operation: tokens and raw parse events.
 */
struct ParseResult {
    std::vector<Token> tokens;
    std::vector<ParseEvent> events;
    bool operator==(const ParseResult& other) const = default;
};

/**
 * @brief A parser that converts a wildcard pattern string into tokens and raw events.
 */
class Parser {
   public:
    /**
     * @brief Parses a pattern string view, generating tokens and raw events.
     * @param p The pattern string view.
     * @return A ParseResult struct containing tokens and raw parse events.
     */
    static ParseResult parse(std::string_view p) {
        ParseResult result;
        if (p.empty()) {
            return result;
        }

        // A temporary builder for merging consecutive literal characters
        std::string literal_builder;

        /**
         * @brief A helper lambda to flush the content of the literal_builder.
         * If the builder contains characters, it creates a LiteralSequence token,
         * adds it to the token list, and clears the builder.
         */
        auto flush_literal_builder = [&]() {
            if (!literal_builder.empty()) {
                result.tokens.push_back(LiteralSequence{std::move(literal_builder)});
                literal_builder.clear();  // Reset for the next sequence
            }
        };

        for (size_t i = 0; i < p.length(); ++i) {
            char current_char = p[i];

            switch (current_char) {
                case '?':
                    flush_literal_builder();
                    result.tokens.push_back(AnyChar{});
                    break;

                case '*':
                    flush_literal_builder();
                    // Merge consecutive '*' by only adding if the previous token wasn't also '*'
                    if (!result.tokens.empty() &&
                        std::holds_alternative<AnySequence>(result.tokens.back())) {
                        result.events.push_back({IssueCode::ConsecutiveAsterisksMerged, i + 1});
                    } else {
                        result.tokens.push_back(AnySequence{});
                    }
                    break;

                case '\\':
                    if (i + 1 < p.length()) {
                        char next_char = p[i + 1];
                        // Check for undefined escape sequences. A "defined" escape is one that
                        // escapes a character with special meaning ('*', '?', '\')
                        if (next_char != '*' && next_char != '?' && next_char != '\\') {
                            result.events.push_back({IssueCode::UndefinedEscapeSequence, i + 1,
                                                     std::string(1, next_char)});
                        }
                        // Still treat as literal for potential recovery
                        literal_builder += next_char;
                        i++;  // Skip the next character in the loop
                    } else {
                        // A trailing backslash event
                        result.events.push_back({IssueCode::TrailingBackslash, i + 1});
                    }
                    break;

                default:
                    // This is a standard literal character
                    literal_builder += current_char;
                    break;
            }
        }

        // After the loop, there might be a pending literal sequence
        flush_literal_builder();

        return result;
    }
};