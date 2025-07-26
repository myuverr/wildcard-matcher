#include <algorithm>
#include <cctype>
#include <string>

#include <gtest/gtest.h>

#include "test_parser_cases.hpp"
#include "utils/parser.hpp"

namespace {

/**
 * @class ParserTest
 * @brief A parameterized test fixture for the Parser class.
 */
class ParserTest : public ::testing::TestWithParam<ParserTestCase> {};

// The single test body that will be executed for each ParserTestCase.
TEST_P(ParserTest, CorrectlyTokenizesAndIdentifiesEvents) {
    const auto& test_case = GetParam();
    SCOPED_TRACE(test_case.description);  // Provides context on failure.

    ParseResult actual_result = Parser::parse(test_case.pattern);
    EXPECT_EQ(actual_result.tokens, test_case.expected_result.tokens);
    EXPECT_EQ(actual_result.events, test_case.expected_result.events);
}

// Instantiate the test suite with all the defined test cases.
// The lambda function generates a valid, descriptive test name from the description field.
INSTANTIATE_TEST_SUITE_P(ParserLogic, ParserTest, ::testing::ValuesIn(parser_test_cases),
                         [](const ::testing::TestParamInfo<ParserTestCase>& info) {
                             std::string name = info.param.description;
                             std::replace_if(
                                 name.begin(), name.end(), [](char c) { return !std::isalnum(c); },
                                 '_');
                             return name;
                         });

}  // namespace