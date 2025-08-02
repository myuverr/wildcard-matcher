#pragma once

#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "utils/compiler.hpp"
#include "utils/issues.hpp"
#include "utils/parser.hpp"

/**
 * @brief Central authority for all input validation and issue creation.
 */
class Validator {
   public:
    /**
     * @brief Validates any raw string for fundamental issues, like multi-byte characters.
     * @param str The string to validate.
     * @return A vector of issues found.
     */
    static std::vector<Issue> validateRawString(std::string_view str) {
        std::vector<Issue> issues;
        for (size_t i = 0; i < str.length(); ++i) {
            // A value > 127 indicates the start of a multi-byte sequence in UTF-8.
            if (static_cast<unsigned char>(str[i]) > 127) {
                issues.push_back(createIssue(IssueCode::MULTIBYTE_CHARACTER_NOT_ALLOWED, i + 1));
                break;  // Stop after the first error.
            }
        }
        return issues;
    }

    /**
     * @brief Interprets events from a ParseResult to generate formal Issues.
     * @param parse_result The result from Parser::parse.
     * @return A vector of all issues (errors and warnings) found during parsing.
     */
    static std::vector<Issue> validateParseResult(const ParseResult& parse_result) {
        std::vector<Issue> issues;
        for (const auto& event : parse_result.events) {
            issues.push_back(createIssue(event.code, event.position, event.detail));
        }
        return issues;
    }

   private:
    /**
     * @brief [private] Factory to create standardized issue messages.
     */
    static Issue createIssue(IssueCode code, size_t position,
                             const std::optional<std::string>& detail = std::nullopt) {
        // Use an immediately-invoked lambda to retrieve the unique details for each case, returning
        // them as a pair of {IssueType, core message string}.
        const auto [type, message_core] = [&] {
            using IssueInfo = std::pair<IssueType, std::string>;
            switch (code) {
                case IssueCode::MULTIBYTE_CHARACTER_NOT_ALLOWED:
                    return IssueInfo{IssueType::ERROR,
                                     "Input must contain only single-byte ASCII characters; a "
                                     "multi-byte character was found."};

                case IssueCode::UNDEFINED_ESCAPE_SEQUENCE:
                    return IssueInfo{
                        IssueType::ERROR,
                        std::format("Undefined escape sequence '\\{}'. This is a fatal error.",
                                    detail.value_or(""))};

                case IssueCode::TRAILING_BACKSLASH:
                    return IssueInfo{
                        IssueType::ERROR,
                        "Pattern cannot end with a trailing backslash. This is a fatal error."};

                case IssueCode::CONSECUTIVE_ASTERISKS_MERGED:
                    return IssueInfo{IssueType::WARNING,
                                     "Consecutive '*' characters were found and automatically "
                                     "merged into a single '*'."};
            }
            // If the switch is exhaustive (as it should be), this code is unreachable.
            APP_UNREACHABLE();
        }();  // <-- Immediately invoke the lambda

        // Centralized message formatting
        std::string message =
            std::format("{} at position {}: {}", issueTypeToString(type), position, message_core);

        return {type, code, message};
    }
};