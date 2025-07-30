#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

/**
 * @brief Defines the types of tokens that can be produced by the parser.
 *
 * This enum provides clear, semantic names for each component of the parsed pattern.
 */
enum class TokenType {
    LITERAL_SEQUENCE,  // Represents a sequence of one or more literal characters.
    ANY_CHAR,          // Represents the '?' wildcard, which matches any single character.
    ANY_SEQUENCE       // Represents the '*' wildcard, which matches any sequence of characters.
};

/**
 * @brief Represents a token in the parsed pattern.
 *
 * Each token has a type and an optional value. The value is used for LITERAL_SEQUENCE
 * to store the actual string of characters.
 */
struct Token {
    TokenType type;
    // Stores the character sequence for LITERAL_SEQUENCE tokens.
    std::optional<std::string> value = std::nullopt;
    bool operator==(const Token& other) const = default;
};

/**
 * @brief Defines machine-readable codes for different parsing warnings.
 */
enum class WarningCode {
    UNDEFINED_ESCAPE_SEQUENCE,  // For sequences like '\a' where 'a' is not a special char
    TRAILING_BACKSLASH          // For a backslash at the very end of the pattern
};

/**
 * @brief A struct to encapsulate a single warning message.
 *
 * It contains a machine-readable code, a human-readable message, and the 0-based
 * index in the raw pattern string where the issue was found.
 */
struct Warning {
    WarningCode code;
    std::string message;
    size_t position;
};

/**
 * @brief A struct to hold the complete result of a parsing operation.
 */
struct ParseResult {
    std::vector<Token> tokens;
    std::vector<Warning> warnings;
};

/**
 * @brief A parser that converts a wildcard pattern string into a sequence of tokens.
 */
class Parser {
   public:
    /**
     * @brief Parses a pattern string view into a structured result containing tokens and warnings.
     * @param p The pattern string view containing literals, wildcards ('?', '*'), and escape
     * sequences.
     * @return A ParseResult struct containing the tokenized pattern and a vector of warnings.
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
         * If the builder contains characters, it creates a LITERAL_SEQUENCE token,
         * adds it to the token list, and clears the builder.
         */
        auto flush_literal_builder = [&]() {
            if (!literal_builder.empty()) {
                result.tokens.push_back({TokenType::LITERAL_SEQUENCE, std::move(literal_builder)});
                literal_builder.clear();  // Reset for the next sequence
            }
        };

        for (size_t i = 0; i < p.length(); ++i) {
            char current_char = p[i];

            switch (current_char) {
                case '?':
                    flush_literal_builder();
                    result.tokens.push_back({TokenType::ANY_CHAR});
                    break;

                case '*':
                    flush_literal_builder();
                    // Merge consecutive '*' by only adding if the previous token wasn't also '*'
                    if (result.tokens.empty() ||
                        result.tokens.back().type != TokenType::ANY_SEQUENCE) {
                        result.tokens.push_back({TokenType::ANY_SEQUENCE});
                    }
                    break;

                case '\\':
                    // --- Updated Escape Sequence Handling with Warnings ---
                    if (i + 1 < p.length()) {
                        char next_char = p[i + 1];
                        // Check for undefined escape sequences. A "defined" escape is one that
                        // escapes a character with special meaning ('*', '?', '\')
                        if (next_char != '*' && next_char != '?' && next_char != '\\') {
                            result.warnings.push_back({WarningCode::UNDEFINED_ESCAPE_SEQUENCE,
                                                       "Undefined escape sequence '\\" +
                                                           std::string(1, next_char) +
                                                           "'. The backslash is ignored and the "
                                                           "character is treated as a "
                                                           "literal.",
                                                       i});
                        }
                        // Regardless of warning, the escaped character is treated as a literal
                        literal_builder += next_char;
                        i++;  // Skip the next character in the loop
                    } else {
                        // A trailing backslash is now a formal warning
                        result.warnings.push_back(
                            {WarningCode::TRAILING_BACKSLASH,
                             "Pattern ends with a trailing backslash. It is treated as a literal "
                             "'\\' character.",
                             i});
                        // The trailing backslash itself is treated as a literal
                        literal_builder += current_char;
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