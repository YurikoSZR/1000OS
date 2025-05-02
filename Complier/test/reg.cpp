#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string text = "联系邮箱：user@example.com，电话：123-456-7890";
    
    // 匹配邮箱
    std::regex emailRegex(R"((\w+)@(\w+\.\w+))");
    std::smatch emailMatch;
    if (std::regex_search(text, emailMatch, emailRegex)) {
        std::cout << "邮箱：" << emailMatch.str(0) << std::endl;
    }

    // 提取所有数字
    std::regex numRegex(R"(\d+)");
    auto numBegin = std::sregex_iterator(text.begin(), text.end(), numRegex);
    for (auto it = numBegin; it != std::sregex_iterator(); ++it) {
        std::cout << "数字：" << it->str() << std::endl;
    }

    return 0;
}
