#include <iostream>
#include <string>

#include "../include/wildcard_matcher.h"

int main() {
    std::string s, p;

    std::cout << "请输入文本字符串 (s): ";
    std::cin >> s;

    std::cout << "请输入模式字符串 (p): ";
    std::cin >> p;

    bool result = isMatch(s.c_str(), p.c_str());

    if (result) {
        std::cout << "结果: 匹配成功" << std::endl;
    } else {
        std::cout << "结果: 匹配失败" << std::endl;
    }

    return 0;
}