#pragma once

#include <string>
#include <string_view>

#include "utils/compiler.hpp"

/**
 * @brief Categorizes an issue as either a non-fatal warning or a fatal error.
 */
enum class IssueType { Warning, Error };

/**
 * @brief Provides a string representation for an IssueType.
 * @param type The issue type.
 * @return A string_view literal for the specified type.
 */
inline std::string_view issueTypeToString(IssueType type) {
    switch (type) {
        case IssueType::Error:
            return "Error";
        case IssueType::Warning:
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
    MultibyteCharacterNotAllowed,

    // --- Parsing Issues ---
    UndefinedEscapeSequence,
    TrailingBackslash,
    ConsecutiveAsterisksMerged
};

/**
 * @brief A unified structure to represent any issue (error or warning) found in the program.
 */
struct Issue {
    IssueType type;
    IssueCode code;
    std::string message;

    // Helper to check if the issue is a fatal error.
    bool isError() const { return type == IssueType::Error; }
};