#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

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
    static std::vector<Issue> validateRawString(std::string_view str);

    /**
     * @brief Interprets events from a ParseResult to generate formal Issues.
     * @param parse_result The result from Parser::parse.
     * @return A vector of all issues (errors and warnings) found during parsing.
     */
    static std::vector<Issue> validateParseResult(const ParseResult& parse_result);

   private:
    /**
     * @brief [private] Factory to create standardized issue messages.
     */
    static Issue createIssue(IssueCode code, std::size_t position,
                             const std::optional<std::string>& detail = std::nullopt);
};