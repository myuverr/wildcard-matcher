#pragma once

#include <string>
#include <vector>

/**
 * @struct WildcardMatchTestCase
 * @brief  Defines a single test case for the wildcard matching problem.
 */
struct WildcardMatchTestCase {
    std::string text;
    std::string pattern;
    bool expected_result;
    std::string description;
};

// A comprehensive collection of test cases to validate solver correctness.
const std::vector<WildcardMatchTestCase> test_cases = {
    // --- Basic & Boundary Cases ---
    {"test", "test", true, "Exact match: No wildcards, identical strings."},
    {"test", "text", false, "Exact mismatch: No wildcards, different characters."},
    {"test", "testing", false, "Length mismatch: No wildcards, different lengths."},
    {"", "", true, "Double empty: Empty string and empty pattern."},
    {"a", "", false, "Empty pattern: Non-empty string with an empty pattern."},
    {"", "a", false, "Empty string: Empty string with a non-empty pattern."},
    {"", "*", true, "Empty string with star: '*' matches an empty sequence."},
    {"", "?", false, "Empty string with question mark: '?' must match one character."},
    {"", "**", true,
     "Empty string with consecutive stars: Multiple '*' are equivalent to one and match an empty "
     "sequence."},
    {"", "*?", false, "Empty string with star and question mark: '?' cannot be matched."},

    // --- '?' Wildcard Cases ---
    {"test", "te?t", true, "Middle match: '?' in the middle of the pattern."},
    {"test", "?est", true, "Start match: '?' at the beginning of the pattern."},
    {"test", "tes?", true, "End match: '?' at the end of the pattern."},
    {"test", "????", true, "Multiple question marks: Matches a string of the same length."},
    {"test", "t?st", true, "Context match: '?' correctly matches 'e'."},

    // --- '*' Wildcard Cases ---
    // 1. '*' Wildcard Basic Behavior
    {"ab", "a*b", true, "Matching empty sequence: '*' matches zero characters."},
    {"acb", "a*b", true, "Matching single character: '*' matches 'c'."},
    {"axyzb", "a*b", true, "Matching multiple characters: '*' matches 'xyz'."},
    {"aa", "a*", true, "Trailing star: '*' at the end matches 'a'."},
    {"aa", "*a", true, "Leading star: '*' at the beginning matches 'a'."},
    {"abc", "a**c", true, "Consecutive stars: '**' is equivalent to '*' and matches 'b'."},
    {"abc", "*", true, "Full star pattern: A single '*' matches the entire string."},
    {"abc", "***", true, "Multiple star pattern: Multiple '*' match the entire string."},

    // 2. '*' Wildcard Backtracking & Non-Greedy Behavior
    {"ab", "*ab", true,
     "Backtracking to match empty: '*' must match empty, not 'a', for the full pattern to match."},
    {"aab", "a*ab", true,
     "Core backtracking: A greedy '*' would match 'aa', causing failure. It must backtrack to "
     "match only one 'a'."},
    {"mississippi", "m*iss*pi", true,
     "Classic complex backtracking: First '*' must match 'iss', second must match 'issip'."},
    {"abacde", "a*cde", true,
     "Backtracking to discard characters: '*' must match 'ba', not the greedy 'bac'."},
    {"bb", "b*b", true, "Deceptively possible match: '*' matches an empty string."},
    {"aaa", "a*a", true, "Middle match: '*' matches the middle 'a'."},

    // --- Mixed Wildcards ---
    {"abc", "a*?c", true, "'*' matches empty, '?' matches 'b'."},
    {"abcc", "a*?c", true, "'*' matches 'b', '?' matches 'c'."},
    {"ac", "a*?c", false, "'*' matches empty, but '?' requires a character that is not there."},
    {"axc", "a?*c", true, "'?' matches 'x', '*' matches empty."},
    {"axbyc", "a?*c", true, "'?' matches 'x', '*' matches 'by'."},
    {"abacde", "a*c?e", true,
     "Forced backtracking: Greedy '*' match ('abac') fails. Must backtrack to match 'aba' so '?' "
     "can match 'd'."},
    {"adceb", "*a*b", true, "First '*' matches empty, second '*' matches 'dce'."},
    {"abc", "?*", true, "'?' matches 'a', '*' matches 'bc'."},

    // --- Logical Traps & Counter-intuitive Cases ---
    {"a", "ab*", false,
     "Pattern mismatch mid-way: 'b' in pattern cannot be matched in the string."},
    {"ba", "*a?", false,
     "Insufficient characters at end: After matching 'a', '?' requires another character which is "
     "not present."},
    {"b", "?*b", false,
     "Insufficient characters at start: '?' requires a character before 'b', which is not "
     "present."},
    {"acdcb", "a*c?b", false,
     "Backtracking trap: No matter if '*' matches 'd' or 'cd', the remaining pattern 'c?b' cannot "
     "match 'cb'."},
    {"b", "*a*", false, "Required character missing: String does not contain 'a'."},

    // --- Large & Complex Cases ---
    {std::string(30, 'a') + "b",
     [] {
         std::string p;
         p.reserve(30);
         for (int i = 0; i < 15; ++i) {
             p += "a*";
         }
         p.pop_back();  // Replace the last '*'
         p += "b";      // with 'b'
         return p;
     }(),
     true,
     "Deep recursion & combinatorial explosion: Forces a naive recursive solver to explore a huge "
     "state space. Aims to test performance when a valid path exists but is hard to find."},
    {"b" + std::string(50, 'a') + "c", "*a*c", true,
     "Greedy trap & multiple backtrack points: A greedy first '*' would match 'b' and all 'a's, "
     "causing failure. It must backtrack to only match 'b' and let the second '*' match the 'a's."},
    {std::string(30, 'a'),
     [] {
         std::string p;
         p.reserve(20);
         for (int i = 0; i < 10; ++i) {
             p += "a*";
         }
         p.pop_back();  // Replace the last '*'
         p += "b";      // with 'b'
         return p;
     }(),
     false,
     "Deep backtracking to ultimate failure: Forces deep exploration of paths that ultimately fail "
     "due to a final character mismatch, testing performance on 'near matches'."},
    {std::string(500, 'a'), "*" + std::string(499, 'a'), true,
     "Large-scale DP table: Tests space and time efficiency of DP solutions on very long strings "
     "and patterns."},
    {[] {
         std::string s;
         s.reserve(1000);
         for (int i = 0; i < 500; ++i) {
             s += "ab";
         }
         return s;
     }(),
     "*a*b*", true,
     "Long string with a sparse pattern: Tests the efficiency of '*' implementation when matching "
     "a general pattern over a long string."},
    {std::string(1000, 'b'), "a*", false,
     "Fast failure: Tests if the algorithm can fail quickly when the pattern's first character "
     "doesn't match, avoiding an unnecessary scan of a long string."},
    {[] {
         std::string s;
         s.reserve(101);
         for (int i = 0; i < 50; ++i) {
             s += "ab";
         }
         s += "c";
         return s;
     }(),
     [] {
         std::string p;
         p.reserve(151);
         for (int i = 0; i < 50; ++i) {
             p += "a*b";
         }
         p += "c";
         return p;
     }(),
     true,
     "Greedy algorithm stability: Each '*' must correctly match an empty string. Tests stability "
     "with a highly repetitive and structured pattern."}};