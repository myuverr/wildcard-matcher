#pragma once

/**
 * @brief 双指针贪心法求解器
 */
struct GreedySolver {
    /**
     * @brief 使用双指针贪心法检查字符串 s 和模式串 p 是否匹配.
     *
     * 使用 s_ptr, p_ptr 遍历. 遇到 '*' 时, 记录其位置 (star_ptr)
     * 和对应的 s 的位置 (s_match_ptr). 若后续失配, 则回溯 (backtrack) 到
     * star_ptr, 并让 s_ptr 从 s_match_ptr 的下一位重新开始.
     */
    static bool isMatch(const char* s, const char* p) {
        const char* s_ptr = s;
        const char* p_ptr = p;
        const char* star_ptr = nullptr;     // 记录 p 中最后出现的 '*' pointer
        const char* s_match_ptr = nullptr;  // 记录 s 中用于 backtrack 的 pointer

        while (*s_ptr) {
            // case 1: 字符匹配, 或 p 为 '?'
            if (*p_ptr == '?' || *p_ptr == *s_ptr) {
                s_ptr++;
                p_ptr++;
            }
            // case 2: p 为 '*', 记录 backtrack 位置
            else if (*p_ptr == '*') {
                star_ptr = p_ptr;
                p_ptr++;
                s_match_ptr = s_ptr;  // 记录 s 的匹配位置
            }
            // case 3: 不匹配, 但之前有 '*', 执行 backtrack
            else if (star_ptr) {
                p_ptr = star_ptr + 1;  // p_ptr 回溯到 '*' 的下一个位置
                s_match_ptr++;         // s_match_ptr 后移, 意味着 '*' 多匹配一个字符
                s_ptr = s_match_ptr;   // s_ptr 从新的 s_match_ptr 位置开始
            }
            // case 4: 不匹配, 且无 '*' 可回溯
            else {
                return false;
            }
        }

        // s 已耗尽, 清理 p 尾部的所有 '*'
        while (*p_ptr == '*') {
            p_ptr++;
        }

        // 如果 p 也被耗尽, 则匹配成功
        return !*p_ptr;
    }
};