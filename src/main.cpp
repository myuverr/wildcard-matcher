#include <cstdlib>
#include <iostream>
#include <string>

#include "../include/dp_solver.hpp"
#include "../include/greedy_solver.hpp"
#include "../include/memo_solver.hpp"
#include "../include/nfa_solver.hpp"
#include "../include/recursive_solver.hpp"
#include "../include/validator.hpp"
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
    size_t invalid_pos;

    std::cout << "Enter the text string (s): ";
    std::getline(std::cin, s);

    // The text string 's' must not contain wildcard characters.
    invalid_pos = InputValidator::findWildcard(s);
    if (invalid_pos != std::string::npos) {
        std::cerr << "Validation Error: The text string 's' cannot contain wildcard characters"
                  << std::endl;
        std::cerr << "  - Found '" << s[invalid_pos] << "' at position " << invalid_pos + 1
                  << std::endl;
        return EXIT_FAILURE;
    }

    // The text string 's' must contain only standard ASCII characters.
    invalid_pos = InputValidator::findMultiByteChar(s);
    if (invalid_pos != std::string::npos) {
        std::cerr << "Validation Error: The text string 's' must contain only single-byte ASCII "
                     "characters"
                  << std::endl;
        std::cerr << "  - Found multi-byte character starting at position " << invalid_pos + 1
                  << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Enter the pattern string (p): ";
    std::getline(std::cin, p);

    // The pattern string 'p' must contain only standard ASCII characters.
    invalid_pos = InputValidator::findMultiByteChar(p);
    if (invalid_pos != std::string::npos) {
        std::cerr << "Validation Error: The pattern string 'p' must contain only single-byte ASCII "
                     "characters"
                  << std::endl;
        std::cerr << "  - Found multi-byte character starting at position " << invalid_pos + 1
                  << std::endl;
        return EXIT_FAILURE;
    }

    SolverProfile profile = runSolver<SelectedSolver>(s, p);

    if (profile.result) {
        std::cout << "Result: Match Successful" << std::endl;
    } else {
        std::cout << "Result: Match Failed" << std::endl;
    }

    std::cout << "Performance Metrics:" << std::endl;
    std::cout << "  - Execution Time: " << profile.time_elapsed_us << " us" << std::endl;
    std::cout << "  - Extra Space: " << profile.space_used_bytes << " bytes" << std::endl;

    return EXIT_SUCCESS;
}