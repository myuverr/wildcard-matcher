#pragma once

#include <optional>
#include <string>
#include <string_view>
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
    static ParseResult parse(std::string_view p);
};