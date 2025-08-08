#pragma once

#include <bitset>
#include <cstdint>
#include <string>
#include <variant>

// --- Master Token List (X-Macro) ---

// To add a new token, add a line here
// Format: APPLY_REST(TypeName)
#define TOKEN_LIST(APPLY_FIRST, APPLY_REST) \
    APPLY_FIRST(AnySequence)                \
    APPLY_REST(LiteralSequence)             \
    APPLY_REST(AnyChar)                     \
    APPLY_REST(CharacterSet)

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

/**
 * @brief Represents a character set token, e.g., `[abc]`, `[a-z]`, or `[^0-9]`.
 *
 * This token matches any single character that is (or is not, if negated)
 * in the defined set.
 */
struct CharacterSet {
    // A bitset where each bit corresponds to an ASCII character code
    // The value 'true' indicates the character is in the set
    std::bitset<256> characters;
    bool operator==(const CharacterSet& other) const = default;
};

// --- Generated Token Constructs ---

// Define the macros for expanding the list
#define EXPAND_TYPE_FIRST(TypeName) TypeName
#define EXPAND_TYPE_REST(TypeName) , TypeName

/**
 * @brief Enum representing the index of each token type in the Token variant.
 */
enum class TokenTypeIndex : std::uint8_t { TOKEN_LIST(EXPAND_TYPE_FIRST, EXPAND_TYPE_REST) };

/**
 * @brief A type-safe variant representing any possible token in the pattern.
 */
using Token = std::variant<TOKEN_LIST(EXPAND_TYPE_FIRST, EXPAND_TYPE_REST)>;

// Clean up the macros
#undef EXPAND_TYPE_FIRST
#undef EXPAND_TYPE_REST