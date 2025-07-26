#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include <cxxopts.hpp>

#include "solvers/dp.hpp"
#include "solvers/greedy.hpp"
#include "solvers/memo.hpp"
#include "solvers/recursive.hpp"
#include "utils/validator.hpp"
#include "wildcard_matcher.hpp"

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
    {"greedy",
     {"Greedy Two-Pointer", "Two-pointer greedy algorithm (default).",
      [](const auto& s, const auto& p) { return runSolver<GreedySolver>(s, p); }}}};

int main(int argc, char* argv[]) {
    // --- Command-Line Argument Parsing Setup with cxxopts ---
    cxxopts::Options options(
        "wildcard_matcher",
        "A program to match text against a wildcard pattern using various algorithms.");

    // Improved help text to clarify the relationship between <arg> and the list of solvers.
    options.add_options()("h,help", "Show this help message and exit.")(
        "s,solver",
        "Specify the solver algorithm. <arg> must be one of the names listed in 'Available "
        "solvers'.",
        cxxopts::value<std::string>()->default_value("greedy"));

    // Helper lambda to print usage information consistently.
    auto print_usage = [&options]() {
        std::cout << options.help() << std::endl;
        std::cout << "Available solvers:" << std::endl;
        for (const auto& [name, info] : solver_registry) {
            printf("  - %-10s: %s\n", name.c_str(), info.description.c_str());
        }
    };

    cxxopts::ParseResult result;
    // --- Robustly parse arguments and handle any exceptions --
    try {
        result = options.parse(argc, argv);
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl << std::endl;
        print_usage();
        return EXIT_FAILURE;
    }

    // --- Handle Help Argument ---
    if (result.count("help")) {
        print_usage();
        return EXIT_SUCCESS;
    }

    // --- Retrieve and Validate Solver Choice ---
    const std::string solver_choice = result["solver"].as<std::string>();
    auto it = solver_registry.find(solver_choice);

    if (it == solver_registry.end()) {
        std::cerr << "Error: Unknown solver '" << solver_choice << "' specified." << std::endl
                  << std::endl;
        print_usage();
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

    // Input validation for the text string 's'
    invalid_pos = InputValidator::findWildcard(s);
    if (invalid_pos != std::string::npos) {
        std::cerr << "Validation Error: The text string 's' cannot contain wildcard characters"
                  << std::endl;
        std::cerr << "  - Found '" << s[invalid_pos] << "' at position " << invalid_pos + 1
                  << std::endl;
        return EXIT_FAILURE;
    }

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

    // Input validation for the pattern string 'p'
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