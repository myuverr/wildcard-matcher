#pragma once

// --- Algorithm Selection ---
// 1: Recursive Backtracking
// 2: Dynamic Programming
// 3: State Machine
// 4: Two-Pointer Greedy
#define SOLVER_CHOICE 2

// --- Include the selected solver ---
#if SOLVER_CHOICE == 1
#include "recursive_solver.hpp"
#elif SOLVER_CHOICE == 2
#include "dp_solver.hpp"
#elif SOLVER_CHOICE == 3
#include "nfa_solver.hpp"
#elif SOLVER_CHOICE == 4
#include "greedy_solver.hpp"
#else
#error \
    "Invalid SOLVER_CHOICE in include/wildcard_matcher.hpp. Please choose a value between 1 and 4."
#endif

// --- Function Declaration ---
// The main matching function exposed to the application.
bool isMatch(const char *s, const char *p);