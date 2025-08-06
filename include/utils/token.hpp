#pragma once

#include <string>
#include <variant>

// --- Master Token List (X-Macro) ---

// To add a new token, add a line here
// Format: APPLY_REST(TypeName)
#define TOKEN_LIST(APPLY_FIRST, APPLY_REST) \
    APPLY_FIRST(AnySequence)                \
    APPLY_REST(LiteralSequence)             \
    APPLY_REST(AnyChar)

// --- Token Data Structures ---

/**
 * @brief Represents a sequence of one or more literal characters.
 *
 * This token matches an exact substring in the input text.
 */
struct LiteralSequence {
    std::string value;
    bool operator==(const LiteralSequence& other) const = default;
};

/**
 * @brief Represents the '?' wildcard.
 *
 * This token matches any single character.
 */
struct AnyChar {
    bool operator==(const AnyChar& other) const = default;
};

/**
 * @brief Represents the '*' wildcard.
 *
 * This token matches any sequence of zero or more characters.
 */
struct AnySequence {
    bool operator==(const AnySequence& other) const = default;
};

// --- Generated Token Constructs ---

// Define the macros for expanding the list
#define EXPAND_TYPE_FIRST(TypeName) TypeName
#define EXPAND_TYPE_REST(TypeName) , TypeName

/**
 * @brief Enum representing the index of each token type in the Token variant.
 */
enum class TokenTypeIndex { TOKEN_LIST(EXPAND_TYPE_FIRST, EXPAND_TYPE_REST) };

/**
 * @brief A type-safe variant representing any possible token in the pattern.
 */
using Token = std::variant<TOKEN_LIST(EXPAND_TYPE_FIRST, EXPAND_TYPE_REST)>;

// Clean up the macros
#undef EXPAND_TYPE_FIRST
#undef EXPAND_TYPE_REST