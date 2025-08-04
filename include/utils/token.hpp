#pragma once

#include <string>
#include <variant>

/**
 * @brief Represents a sequence of one or more literal characters.
 */
struct LiteralSequence {
    std::string value;
    bool operator==(const LiteralSequence& other) const = default;
};

/**
 * @brief Represents the '?' wildcard, which matches any single character.
 */
struct AnyChar {
    bool operator==(const AnyChar& other) const = default;
};

/**
 * @brief Represents the '*' wildcard, which matches any sequence of characters.
 */
struct AnySequence {
    bool operator==(const AnySequence& other) const = default;
};

// Using std::variant to represent any possible token type.
using Token = std::variant<LiteralSequence, AnyChar, AnySequence>;