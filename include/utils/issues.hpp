#pragma once

#include <string>
#include <string_view>

#include "utils/compiler.hpp"

/**
 * @brief Categorizes an issue as either a non-fatal warning or a fatal error.
 */
enum class IssueType { WARNING, ERROR };

/**
 * @brief Provides a string representation for an IssueType.
 * @param type The issue type.
 * @return A string_view literal for the specified type.
 */
inline std::string_view issueTypeToString(IssueType type) {
    switch (type) {
        case IssueType::ERROR:
            return "Error";
        case IssueType::WARNING:
            return "Warning";
    }
    // This path is unreachable if all enum values are handled in the switch.
    APP_UNREACHABLE();
}

/**
 * @brief Provides machine-readable codes for all possible validation and parsing issues.
 */
enum class IssueCode {
    // --- Validation Issues ---
    MULTIBYTE_CHARACTER_NOT_ALLOWED,

    // --- Parsing Issues ---
    UNDEFINED_ESCAPE_SEQUENCE,
    TRAILING_BACKSLASH,
    CONSECUTIVE_ASTERISKS_MERGED
};

/**
 * @brief A unified structure to represent any issue (error or warning) found in the program.
 */
struct Issue {
    IssueType type;
    IssueCode code;
    std::string message;

    // Helper to check if the issue is a fatal error.
    bool isError() const { return type == IssueType::ERROR; }
};