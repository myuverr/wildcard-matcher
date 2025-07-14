#include <iostream>
#include <string>

#include "../include/dp_solver.hpp"
#include "../include/greedy_solver.hpp"
#include "../include/memo_solver.hpp"
#include "../include/nfa_solver.hpp"
#include "../include/recursive_solver.hpp"
#include "../include/wildcard_matcher.hpp"

int main() {
    /**
     * @brief Defines the type alias for the core matching algorithm.
     *
     * Available solver types include:
     * - RecursiveSolver: Implements the recursive backtracking algorithm.
     * - MemoSolver:      Implements the memoized recursion algorithm.
     * - DpSolver:        Implements the dynamic programming algorithm.
     * - NFASolver:       Implements the state machine-based (space-optimized DP) algorithm.
     * - GreedySolver:    Implements the two-pointer greedy algorithm.
     */
    using SelectedSolver = GreedySolver;

    std::string s, p;

    std::cout << "Enter the text string (s): ";
    std::cin >> s;

    std::cout << "Enter the pattern string (p): ";
    std::cin >> p;

    SolverProfile profile = runSolver<SelectedSolver>(s.c_str(), p.c_str());

    if (profile.result) {
        std::cout << "Result: Match Successful" << std::endl;
    } else {
        std::cout << "Result: Match Failed" << std::endl;
    }

    std::cout << "Performance Metrics:" << std::endl;
    std::cout << "  - Execution Time: " << profile.time_elapsed_us << " μs" << std::endl;
    std::cout << "  - Extra Space: " << profile.space_used_bytes << " bytes" << std::endl;

    return 0;
}