#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

#include "utils/compiler.hpp"

/**
 * @brief Categorizes an issue as either a non-fatal warning or a fatal error.
 */
enum class IssueType : std::uint8_t { Warning, Error };

/**
 * @brief Specialization of std::formatter for the IssueType enum.
 */
template <>
struct std::formatter<IssueType> : std::formatter<std::string_view> {
    /**
     * @brief Formats the IssueType enum as a string.
     * @param type The IssueType value to format.
     * @param ctx The formatting context.
     * @return An iterator to the end of the formatted output.
     */
    auto format(IssueType type, auto& ctx) const {
        const string_view name = [&] {
            switch (type) {
                case IssueType::Error: {
                    return "Error";
                }
                case IssueType::Warning: {
                    return "Warning";
                }
            }
            APP_UNREACHABLE();
        }();

        // Delegate the actual formatting to the base class formatter for std::string_view
        return std::formatter<std::string_view>::format(name, ctx);
    }
};

/**
 * @brief Provides machine-readable codes for all possible validation and parsing issues.
 */
enum class IssueCode : std::uint8_t {
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