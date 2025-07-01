#include <iostream>
#include <string>

// 引入通用 matcher 和所有可用的 solver 实现
#include "../include/dp_solver.hpp"
#include "../include/greedy_solver.hpp"
#include "../include/nfa_solver.hpp"
#include "../include/recursive_solver.hpp"
#include "../include/wildcard_matcher.hpp"

int main() {
    // --- 算法选择 ---
    // 通过指定类型来选择 solver. 这是一个编译时决策.
    using SelectedSolver = RecursiveSolver;
    // using SelectedSolver = DpSolver;

    std::string s, p;

    std::cout << "请输入文本字符串 (s): ";
    std::cin >> s;

    std::cout << "请输入模式字符串 (p): ";
    std::cin >> p;

    bool result = isMatch<SelectedSolver>(s.c_str(), p.c_str());

    if (result) {
        std::cout << "结果: 匹配成功" << std::endl;
    } else {
        std::cout << "结果: 匹配失败" << std::endl;
    }

    return 0;
}