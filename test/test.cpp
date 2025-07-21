#include <gtest/gtest.h>

#include "dp_solver.hpp"
#include "greedy_solver.hpp"
#include "memo_solver.hpp"
#include "nfa_solver.hpp"
#include "recursive_solver.hpp"
#include "test_cases.hpp"
#include "wildcard_matcher.hpp"

/**
 * @class WildcardSolverTest
 * @brief A type-parameterized test fixture for wildcard matchers.
 *
 * This fixture is templated on a `TypeParam`, which is expected to be a
 * solver class that conforms to the `WildcardSolver` concept. This allows
 * the same test logic to be applied to any valid solver implementation.
 *
 * @tparam T A class that implements the solver interface.
 */
template <typename T>
class WildcardSolverTest : public ::testing::Test {};

// Declare the type-parameterized test suite.
TYPED_TEST_SUITE_P(WildcardSolverTest);

/**
 * @brief The core test logic executed for each solver type.
 *
 * This test iterates through a global vector of `WildcardMatchTestCase`
 * instances. For each case, it invokes the `runAndProfile` method of the
 * current solver type (`TypeParam`) and asserts that the result matches
 * the expected outcome. `SCOPED_TRACE` is used to provide detailed context
 * for any test failures, including the test case description and inputs.
 */
TYPED_TEST_P(WildcardSolverTest, MatchesAccordingToDefinedCases) {
    using SolverType = TypeParam;

    // Verify that the provided type conforms to the concept.
    static_assert(WildcardSolver<SolverType>, "TypeParam must be a valid WildcardSolver.");

    for (const auto& test_case : test_cases) {
        SCOPED_TRACE((testing::Message()
                      << "Test Case: " << test_case.description << "\n  s: \"" << test_case.text
                      << "\"" << "\n  p: \"" << test_case.pattern << "\""));

        SolverProfile profile = SolverType::runAndProfile(test_case.text, test_case.pattern);
        EXPECT_EQ(profile.result, test_case.expected_result);
    }
}

// Register the test case implementation with the test suite.
// This connects the logic inside the TYPED_TEST_P block to the suite name.
REGISTER_TYPED_TEST_SUITE_P(WildcardSolverTest, MatchesAccordingToDefinedCases);

// A type list containing all solver classes to be tested.
using SolverImplementations =
    ::testing::Types<RecursiveSolver, MemoSolver, DpSolver, NFASolver, GreedySolver>;

// Instantiate the test suite for each type in the SolverImplementations list.
// The first argument is a user-defined prefix for the test suite name in the final output.
INSTANTIATE_TYPED_TEST_SUITE_P(AllSolvers, WildcardSolverTest, SolverImplementations);