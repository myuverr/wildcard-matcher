#pragma once

#include <optional>
#include <string>
#include <vector>

#include "utils/parser.hpp"

// A struct to define a single parser test case.
struct ParserTestCase {
    std::string_view pattern;
    ParseResult expected_result;
    std::string description;
};

// A vector containing all test cases for the parser.
const std::vector<ParserTestCase> parser_test_cases = {
    {"", {{}, {}}, "Empty_pattern_should_result_in_no_tokens_or_events"},
    {"abc", {{{LiteralSequence{"abc"}}}, {}}, "Simple_literal_string"},
    {"a?c",
     {{{LiteralSequence{"a"}, AnyChar{}, LiteralSequence{"c"}}}, {}},
     "ANY_CHAR_wildcard_surrounded_by_literals"},
    {"a*c",
     {{{LiteralSequence{"a"}, AnySequence{}, LiteralSequence{"c"}}}, {}},
     "ANY_SEQUENCE_wildcard_surrounded_by_literals"},
    {"a**c",
     {{{LiteralSequence{"a"}, AnySequence{}, LiteralSequence{"c"}}},
      {{IssueCode::CONSECUTIVE_ASTERISKS_MERGED, 3, std::nullopt}}},
     "Single_consecutive_asterisk_should_be_merged"},
    {"a***c",
     {{{LiteralSequence{"a"}, AnySequence{}, LiteralSequence{"c"}}},
      {{IssueCode::CONSECUTIVE_ASTERISKS_MERGED, 3, std::nullopt},
       {IssueCode::CONSECUTIVE_ASTERISKS_MERGED, 4, std::nullopt}}},
     "Multiple_consecutive_asterisks_should_generate_multiple_events"},
    {"a\\*c", {{{LiteralSequence{"a*c"}}}, {}}, "Escaped_asterisk_should_be_a_literal"},
    {"a\\?c", {{{LiteralSequence{"a?c"}}}, {}}, "Escaped_question_mark_is_a_literal"},
    {"a\\\\c", {{{LiteralSequence{"a\\c"}}}, {}}, "Escaped_backslash_is_a_literal"},
    {"abc\\",
     {{{LiteralSequence{"abc"}}}, {{IssueCode::TRAILING_BACKSLASH, 4, std::nullopt}}},
     "Trailing_backslash_should_produce_an_event"},
    {"a\\nc",
     {{{LiteralSequence{"anc"}}}, {{IssueCode::UNDEFINED_ESCAPE_SEQUENCE, 2, "n"}}},
     "Undefined_escape_sequence_should_produce_an_event"},
    {"*a?b\\*c**d\\",
     {{{AnySequence{}, LiteralSequence{"a"}, AnyChar{}, LiteralSequence{"b*c"}, AnySequence{},
        LiteralSequence{"d"}}},
      {{IssueCode::CONSECUTIVE_ASTERISKS_MERGED, 9, std::nullopt},
       {IssueCode::TRAILING_BACKSLASH, 11, std::nullopt}}},
     "Complex_pattern_with_multiple_wildcards_escapes_and_events"}};