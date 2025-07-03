#pragma once

#include <concepts>

struct SolverProfile {
    bool result;
    long long time_elapsed_us;
    std::size_t space_used_bytes;
};

// --- Concept 定义 ---
// 如果一个类型提供了静态 runAndProfile 方法, 它就满足 WildcardSolver concept.
template <typename T>
concept WildcardSolver = requires(const char* s, const char* p) {
    { T::runAndProfile(s, p) } -> std::same_as<SolverProfile>;
};

// --- 函数声明 ---
// 核心匹配函数, 基于 solver 策略进行模板化.
template <WildcardSolver Solver>
SolverProfile runSolver(const char* s, const char* p) {
    return Solver::runAndProfile(s, p);
}