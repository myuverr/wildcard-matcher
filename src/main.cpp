#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

#include "../include/dp_solver.hpp"
#include "../include/greedy_solver.hpp"
#include "../include/memo_solver.hpp"
#include "../include/nfa_solver.hpp"
#include "../include/recursive_solver.hpp"
#include "../include/validator.hpp"
#include "../include/wildcard_matcher.hpp"

/**
 * @struct SolverInfo
 * @brief  Holds all relevant information for a single solver.
 */
struct SolverInfo {
    std::string fullname;
    std::string description;
    std::function<SolverProfile(const std::string&, const std::string&)> run_function;
};

// Use a static map to act as a central "Solver Registry"
const static std::map<std::string, SolverInfo> solver_registry = {
    {"recursive",
     {"Recursive Backtracking", "Recursive backtracking algorithm.",
      [](const auto& s, const auto& p) { return runSolver<RecursiveSolver>(s, p); }}},
    {"memo",
     {"Memoized Recursion", "Memoized recursion algorithm.",
      [](const auto& s, const auto& p) { return runSolver<MemoSolver>(s, p); }}},
    {"dp",
     {"Dynamic Programming", "Dynamic programming algorithm.",
      [](const auto& s, const auto& p) { return runSolver<DpSolver>(s, p); }}},
    {"nfa",
     {"NFA (State Machine)", "State machine (space-optimized DP) algorithm.",
      [](const auto& s, const auto& p) { return runSolver<NFASolver>(s, p); }}},
    {"greedy",
     {"Greedy Two-Pointer", "Two-pointer greedy algorithm (default).",
      [](const auto& s, const auto& p) { return runSolver<GreedySolver>(s, p); }}}};

void printUsage(const char* prog_name);

int main(int argc, char* argv[]) {
    std::string solver_choice = "greedy";  // Default solver

    // --- Command-Line Argument Parsing ---
    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return EXIT_SUCCESS;
        }
        if (arg == "--solver" || arg == "-s") {
            if (i + 1 < argc) {             // Ensure there is a next argument
                solver_choice = argv[++i];  // Assign next argument and advance index
            } else {
                std::cerr << "Error: --solver or -s option requires an argument" << std::endl;
                printUsage(argv[0]);
                return EXIT_FAILURE;
            }
        } else {
            std::cerr << "Error: Unknown option '" << arg << std::endl;
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    // --- Validate solver choice by looking it up in the registry ---
    auto it = solver_registry.find(solver_choice);
    if (it == solver_registry.end()) {
        std::cerr << "Error: Unknown solver '" << solver_choice << "' specified" << std::endl;
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    // Get the specific solver's info from the registry
    const auto& selected_solver_info = it->second;

    // --- Proceed with user input ---
    std::string s, p;
    size_t invalid_pos;

    std::cout << "Enter the text string (s): ";
    std::getline(std::cin, s);
    if (!std::cin) {
        return EXIT_FAILURE;  // Exit on stream error/closure.
    }

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
    if (!std::cin) {
        return EXIT_FAILURE;  // Exit on stream error/closure.
    }

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

    // --- Run the selected solver using the function from the registry ---
    SolverProfile profile = selected_solver_info.run_function(s, p);

    if (profile.result) {
        std::cout << "Result: Match Successful" << std::endl;
    } else {
        std::cout << "Result: Match Failed" << std::endl;
    }

    std::cout << "Performance Metrics:" << std::endl;
    std::cout << "  - Solver Used: " << selected_solver_info.fullname << std::endl;
    std::cout << "  - Execution Time: " << profile.time_elapsed_us << " us" << std::endl;
    std::cout << "  - Extra Space: " << profile.space_used_bytes << " bytes" << std::endl;

    return EXIT_SUCCESS;
}

/**
 * @brief Prints the usage instructions for the program.
 *
 * The list of available solvers is dynamically generated from the registry.
 *
 * @param prog_name The name of the executable, taken from argv[0].
 */
void printUsage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help                Show this help message and exit." << std::endl;
    std::cout << "  -s, --solver <name>       Specify the solver algorithm to use." << std::endl;
    std::cout << "Available solvers:" << std::endl;
    for (const auto& [name, info] : solver_registry) {
        printf("  - %-10s: %s\n", name.c_str(), info.description.c_str());
    }
}