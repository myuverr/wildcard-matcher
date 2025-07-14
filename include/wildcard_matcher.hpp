#pragma once

#include <concepts>

struct SolverProfile {
    bool result;
    long long time_elapsed_us;
    std::size_t space_used_bytes;
};

// --- Concept Definition ---
// A type satisfies the WildcardSolver concept if it provides a static runAndProfile method.
template <typename T>
concept WildcardSolver = requires(const char* s, const char* p) {
    { T::runAndProfile(s, p) } -> std::same_as<SolverProfile>;
};

// --- Function Declaration ---
// The core matching function, templated based on the solver strategy.
template <WildcardSolver Solver>
SolverProfile runSolver(const char* s, const char* p) {
    return Solver::runAndProfile(s, p);
}