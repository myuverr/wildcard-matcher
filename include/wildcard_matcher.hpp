#pragma once

#include <concepts>
#include <string_view>

struct SolverProfile {
    bool result;
    long long time_elapsed_us;
    std::size_t space_used_bytes;
};

// --- Concept Definition ---
// A type satisfies the WildcardSolver concept if it provides a static runAndProfile method.
template <typename T>
concept WildcardSolver = requires(std::string_view s, std::string_view p) {
    { T::runAndProfile(s, p) } -> std::same_as<SolverProfile>;
};

// --- Function Declaration ---
// The core matching function, templated based on the solver strategy.
template <WildcardSolver Solver>
SolverProfile runSolver(std::string_view s, std::string_view p) {
    return Solver::runAndProfile(s, p);
}