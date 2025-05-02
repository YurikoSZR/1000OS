#include "FA.h"
int main() {
    // 测试用例列表
    vector<string> test_cases = {
        "(a*)|b",
        "(a|b)*d(c|d)*"
    };

    // 执行所有测试
    for (const auto& re : test_cases) {
        run_regex_test(re);
    }

    return 0;
}