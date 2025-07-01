#pragma once

#include <concepts>

// --- Concept 定义 ---
// 如果一个类型提供了静态 isMatch 方法, 它就满足 WildcardSolver concept.
template <typename T>
concept WildcardSolver = requires(const char* s, const char* p) {
    { T::isMatch(s, p) } -> std::same_as<bool>;
};

// --- 函数声明 ---
// 核心匹配函数, 基于 solver 策略进行模板化.
template <WildcardSolver Solver>
bool isMatch(const char* s, const char* p) {
    return Solver::isMatch(s, p);
}