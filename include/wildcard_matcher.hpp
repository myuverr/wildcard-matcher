#pragma once

#include <concepts>

// --- Concept Definition ---
// A type satisfies WildcardSolver if it provides a static isMatch method.
template <typename T>
concept WildcardSolver = requires(const char* s, const char* p) {
    { T::isMatch(s, p) } -> std::same_as<bool>;
};

// --- Function Declaration ---
// The main matching function, templated on the solver strategy.
template <WildcardSolver Solver>
bool isMatch(const char* s, const char* p) {
    return Solver::isMatch(s, p);
}