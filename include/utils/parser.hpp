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
 * @brief A parser that converts a wildcard pattern string into a sequence of tokens.
 */
class Parser {
   public:
    /**
     * @brief Parses a pattern string view into a vector of tokens.
     * @param p The pattern string view containing literals and wildcards ('?', '*').
     * @return A std::vector<Token> representing the structured pattern.
     */
    static std::vector<Token> parse(std::string_view p) {
        std::vector<Token> tokens;
        if (p.empty()) {
            return tokens;
        }

        // A temporary builder for merging consecutive literal characters.
        std::string literal_builder;

        /**
         * @brief A helper lambda to flush the content of the literal_builder.
         * If the builder contains characters, it creates a LITERAL_SEQUENCE token,
         * adds it to the token list, and clears the builder.
         */
        auto flush_literal_builder = [&]() {
            if (!literal_builder.empty()) {
                tokens.push_back({TokenType::LITERAL_SEQUENCE, std::move(literal_builder)});
                literal_builder.clear();  // Reset for the next sequence.
            }
        };

        for (size_t i = 0; i < p.length(); ++i) {
            char current_char = p[i];

            switch (current_char) {
                case '?':
                    flush_literal_builder();
                    tokens.push_back({TokenType::ANY_CHAR});
                    break;

                case '*':
                    flush_literal_builder();
                    // Merge consecutive '*' by only adding if the previous token wasn't also '*'.
                    if (tokens.empty() || tokens.back().type != TokenType::ANY_SEQUENCE) {
                        tokens.push_back({TokenType::ANY_SEQUENCE});
                    }
                    break;

                case '\\':
                    // Handle escape sequence. The next character is treated as a literal.
                    if (i + 1 < p.length()) {
                        literal_builder += p[i + 1];  // Append the escaped character.
                        i++;                          // Skip the next character in the loop.
                    } else {
                        // A trailing backslash is treated as a literal backslash.
                        literal_builder += current_char;
                    }
                    break;

                default:
                    // This is a standard literal character.
                    literal_builder += current_char;
                    break;
            }
        }

        // After the loop, there might be a pending literal sequence.
        flush_literal_builder();

        return tokens;
    }
};