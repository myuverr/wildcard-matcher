// test/test_validator.cpp
#include <gtest/gtest.h>

#include "utils/issues.hpp"
#include "utils/parser.hpp"
#include "utils/validator.hpp"

namespace {

/**
 * @class ValidatorTest
 * @brief A test fixture for the Validator class.
 */
class ValidatorTest : public ::testing::Test {};

// --- Tests for Validator::validateRawString ---

TEST_F(ValidatorTest, AcceptsValidASCIIString) {
    auto issues = Validator::validateRawString("a_valid-string.123?*");
    EXPECT_TRUE(issues.empty());
}

TEST_F(ValidatorTest, DetectsMultiByteCharacter) {
    // A string containing a multi-byte UTF-8 character for the copyright symbol '©'
    auto issues = Validator::validateRawString("abc©def");
    ASSERT_EQ(issues.size(), 1);
    const auto& issue = issues[0];
    EXPECT_EQ(issue.code, IssueCode::MULTIBYTE_CHARACTER_NOT_ALLOWED);
    EXPECT_TRUE(issue.isError());
    EXPECT_NE(issue.message.find("position 4"), std::string::npos)
        << "The error message should pinpoint the correct position.";
}

// --- Tests for Validator::validateParseResult ---

TEST_F(ValidatorTest, ConvertsConsecutiveAsterisksToWarning) {
    ParseResult pr;
    pr.events.push_back({IssueCode::CONSECUTIVE_ASTERISKS_MERGED, 3, std::nullopt});
    auto issues = Validator::validateParseResult(pr);

    ASSERT_EQ(issues.size(), 1);
    const auto& issue = issues[0];
    EXPECT_EQ(issue.code, IssueCode::CONSECUTIVE_ASTERISKS_MERGED);
    EXPECT_EQ(issue.type, IssueType::WARNING);
    EXPECT_NE(issue.message.find("Warning at position 3"), std::string::npos);
}

TEST_F(ValidatorTest, ConvertsTrailingBackslashToError) {
    ParseResult pr;
    pr.events.push_back({IssueCode::TRAILING_BACKSLASH, 5, std::nullopt});
    auto issues = Validator::validateParseResult(pr);

    ASSERT_EQ(issues.size(), 1);
    const auto& issue = issues[0];
    EXPECT_EQ(issue.code, IssueCode::TRAILING_BACKSLASH);
    EXPECT_TRUE(issue.isError());
    EXPECT_NE(issue.message.find("Error at position 5"), std::string::npos);
}

TEST_F(ValidatorTest, ConvertsUndefinedEscapeToError) {
    ParseResult pr;
    pr.events.push_back({IssueCode::UNDEFINED_ESCAPE_SEQUENCE, 2, "x"});
    auto issues = Validator::validateParseResult(pr);

    ASSERT_EQ(issues.size(), 1);
    const auto& issue = issues[0];
    EXPECT_EQ(issue.code, IssueCode::UNDEFINED_ESCAPE_SEQUENCE);
    EXPECT_TRUE(issue.isError());
    EXPECT_NE(issue.message.find("Error at position 2: Undefined escape sequence '\\x'"),
              std::string::npos);
}

TEST_F(ValidatorTest, HandlesMultipleEventsCorrectly) {
    // This pattern has consecutive asterisks (warning) and a trailing backslash (error).
    ParseResult pr;
    pr.events.push_back({IssueCode::CONSECUTIVE_ASTERISKS_MERGED, 3, std::nullopt});
    pr.events.push_back({IssueCode::TRAILING_BACKSLASH, 6, std::nullopt});

    auto issues = Validator::validateParseResult(pr);
    ASSERT_EQ(issues.size(), 2);

    // Order of events is not guaranteed to be preserved, so check for the presence of both.
    // This makes the test more robust.
    bool found_warning = std::any_of(issues.begin(), issues.end(), [](const Issue& i) {
        return i.code == IssueCode::CONSECUTIVE_ASTERISKS_MERGED && i.type == IssueType::WARNING;
    });
    bool found_error = std::any_of(issues.begin(), issues.end(), [](const Issue& i) {
        return i.code == IssueCode::TRAILING_BACKSLASH && i.type == IssueType::ERROR;
    });

    EXPECT_TRUE(found_warning) << "The consecutive asterisks warning was not generated.";
    EXPECT_TRUE(found_error) << "The trailing backslash error was not generated.";
}

}  // namespace