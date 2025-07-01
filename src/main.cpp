#include <iostream>
#include <string>

// Include the generic matcher and all available solver implementations
#include "../include/dp_solver.hpp"
#include "../include/greedy_solver.hpp"
#include "../include/nfa_solver.hpp"
#include "../include/recursive_solver.hpp"
#include "../include/wildcard_matcher.hpp"

int main() {
    // --- Algorithm Selection ---
    // Select the solver by specifying its type. This is a compile-time decision.
    using SelectedSolver = DpSolver;
    // using SelectedSolver = RecursiveSolver;

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