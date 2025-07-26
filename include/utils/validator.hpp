#pragma once

#include <string>
#include <string_view>

/**
 * @brief Provides functions to validate input strings based on project requirements.
 */
class InputValidator {
   public:
    /**
     * @brief Finds the first occurrence of a multi-byte character (non-ASCII).
     * @param str The string_view to validate.
     * @return The index of the first multi-byte character.
     * Returns std::string::npos if no such character is found.
     */
    static size_t findMultiByteChar(std::string_view str) {
        for (size_t i = 0; i < str.length(); ++i) {
            // A value > 127 indicates the start of a multi-byte sequence in UTF-8.
            if (static_cast<unsigned char>(str[i]) > 127) {
                return i;  // Return the index of the problematic character
            }
        }
        return std::string::npos;
    }

    /**
     * @brief Finds the first occurrence of a wildcard character ('?' or '*').
     * @param str The string_view to validate.
     * @return The 0-based index of the first wildcard character.
     * Returns std::string::npos if no such character is found.
     */
    static size_t findWildcard(std::string_view str) {
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '?' || str[i] == '*') {
                return i;  // Return the index of the problematic character
            }
        }
        return std::string::npos;
    }
};