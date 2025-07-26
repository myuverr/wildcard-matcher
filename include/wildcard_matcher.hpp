#pragma once

#include <concepts>
#include <string_view>
#include <vector>

// Forward declaration of Token to avoid circular dependency
struct Token;

struct SolverProfile {
    bool result;
    long long time_elapsed_us;
    std::size_t space_used_bytes;
};

// --- Concept Definition ---
// A type satisfies the WildcardSolver concept if it provides a static runAndProfile method
template <typename T>
concept WildcardSolver = requires(std::string_view s, const std::vector<Token>& p_tokens) {
    { T::runAndProfile(s, p_tokens) } -> std::same_as<SolverProfile>;
};

// --- Function Declaration ---
// The core matching function, templated based on the solver strategy
template <WildcardSolver Solver>
SolverProfile runSolver(std::string_view s, const std::vector<Token>& p_tokens) {
    return Solver::runAndProfile(s, p_tokens);
}