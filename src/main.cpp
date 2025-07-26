#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <cxxopts.hpp>

#include "solvers/dp.hpp"
#include "solvers/greedy.hpp"
#include "solvers/memo.hpp"
#include "solvers/recursive.hpp"
#include "utils/parser.hpp"
#include "utils/validator.hpp"
#include "wildcard_matcher.hpp"

/**
 * @struct SolverInfo
 * @brief  Holds all relevant information for a single solver.
 */
struct SolverInfo {
    std::string fullname;
    std::string description;
    // The function now takes the raw string and a vector of pre-parsed pattern tokens.
    std::function<SolverProfile(const std::string&, const std::vector<Token>&)> run_function;
};

// Use a static map to act as a central "Solver Registry"
const static std::map<std::string, SolverInfo> solver_registry = {
    {"recursive",
     {"Recursive Backtracking", "Recursive backtracking algorithm.",
      [](const auto& s, const auto& p_tokens) { return runSolver<RecursiveSolver>(s, p_tokens); }}},
    {"memo",
     {"Memoized Recursion", "Memoized recursion algorithm.",
      [](const auto& s, const auto& p_tokens) { return runSolver<MemoSolver>(s, p_tokens); }}},
    {"dp",
     {"Dynamic Programming", "Dynamic programming algorithm.",
      [](const auto& s, const auto& p_tokens) { return runSolver<DpSolver>(s, p_tokens); }}},
    {"greedy",
     {"Greedy Two-Pointer", "Two-pointer greedy algorithm (default).",
      [](const auto& s, const auto& p_tokens) { return runSolver<GreedySolver>(s, p_tokens); }}}};

/**
 * @brief Processes a list of issues, printing them and identifying if fatal errors exist.
 *
 * This function handles the logic for reporting warnings and errors found during validation
 * or parsing. It separates issues by type, prints them to std::cerr, and indicates
 * whether the program should terminate.
 *
 * @param issues A vector of Issues to process.
 * @param context_header A descriptive string (e.g., "in the text string") to provide context.
 * @return True if any fatal errors were found, otherwise false.
 */
static bool processAndPrintIssues(const std::vector<Issue>& issues,
                                  const std::string& context_header) {
    if (issues.empty()) {
        return false;  // No issues, no fatal errors.
    }

    std::vector<const Issue*> errors;
    std::vector<const Issue*> warnings;

    // Separate issues into errors and warnings for structured reporting.
    for (const auto& issue : issues) {
        if (issue.isError()) {
            errors.push_back(&issue);
        } else {
            warnings.push_back(&issue);
        }
    }

    // Always report warnings if they exist.
    if (!warnings.empty()) {
        std::cerr << "Warning(s) found " << context_header << ":" << std::endl;
        for (const auto* warning : warnings) {
            std::cerr << "  - " << warning->message << std::endl;
        }
    }

    // If errors exist, report them and signal for termination.
    if (!errors.empty()) {
        std::cerr << "Error(s) found " << context_header
                  << ". Operation cannot continue:" << std::endl;
        for (const auto* error : errors) {
            std::cerr << "  - " << error->message << std::endl;
        }
        return true;  // Fatal errors found.
    }

    return false;  // No fatal errors.
}

int main(int argc, char* argv[]) {
    // --- Command-Line Argument Parsing Setup with cxxopts ---
    cxxopts::Options options(
        "wildcard_matcher",
        "A program to match text against a wildcard pattern using various algorithms.\n"
        "The pattern supports '?' (any single character), '*' (any sequence),\n"
        "and '\\' to escape special characters.");

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

    // --- Get and Validate Text String (s) ---
    std::string s;
    std::cout << "Enter the text string (s): ";
    if (!std::getline(std::cin, s)) {
        return EXIT_FAILURE;  // Exit on stream error/closure.
    }

    // Perform fundamental validation (e.g., for multi-byte characters).
    if (processAndPrintIssues(Validator::validateRawString(s), "in the text string")) {
        return EXIT_FAILURE;
    }

    // --- Get and Validate Pattern String (p) ---
    std::string p;
    std::cout << "Enter the pattern string (p): ";
    if (!std::getline(std::cin, p)) {
        return EXIT_FAILURE;  // Exit on stream error/closure.
    }

    // Perform the same fundamental validation on the pattern string.
    if (processAndPrintIssues(Validator::validateRawString(p), "in the pattern string")) {
        return EXIT_FAILURE;
    }

    // --- Parse the Pattern and Validate its Structure ---
    ParseResult parse_result = Parser::parse(p);
    auto parse_issues = Validator::validateParseResult(parse_result);

    // Process any warnings (e.g., merged asterisks) or fatal errors (e.g., bad escape sequence).
    if (processAndPrintIssues(parse_issues, "during pattern parsing")) {
        return EXIT_FAILURE;
    }

    // --- Run the selected solver ---
    SolverProfile profile = selected_solver_info.run_function(s, parse_result.tokens);

    // --- Print results ---
    std::cout << "\nResult: " << (profile.result ? "Match Successful" : "Match Failed")
              << std::endl;

    std::cout << "Performance Metrics:" << std::endl;
    std::cout << "  - Solver Used: " << selected_solver_info.fullname << std::endl;
    std::cout << "  - Execution Time: " << profile.time_elapsed_us << " us" << std::endl;
    std::cout << "  - Extra Space: " << profile.space_used_bytes << " bytes" << std::endl;

    return EXIT_SUCCESS;
}