#include <iostream>
#include <string>

#include "../include/dp_solver.hpp"
#include "../include/greedy_solver.hpp"
#include "../include/nfa_solver.hpp"
#include "../include/recursive_solver.hpp"
#include "../include/wildcard_matcher.hpp"

int main() {
    /**
     * @brief 定义核心求解器的类型别名。
     *
     * 可选的求解器类型包括：
     * - `RecursiveSolver`: 递归与回溯求解器
     * - `DpSolver`:        动态规划求解器
     * - `NFASolver`:       基于状态机的求解器
     * - `GreedySolver`:    双指针贪心法求解器
     */
    using SelectedSolver = DpSolver;

    std::string s, p;

    std::cout << "请输入文本字符串 (s): ";
    std::cin >> s;

    std::cout << "请输入模式字符串 (p): ";
    std::cin >> p;

    SolverProfile profile = runSolver<SelectedSolver>(s.c_str(), p.c_str());

    if (profile.result) {
        std::cout << "结果: 匹配成功" << std::endl;
    } else {
        std::cout << "结果: 匹配失败" << std::endl;
    }

    std::cout << "性能指标:" << std::endl;
    std::cout << "  - 执行耗时: " << profile.time_elapsed_us << " μs" << std::endl;
    std::cout << "  - 额外空间: " << profile.space_used_bytes << " bytes" << std::endl;

    return 0;
}