#include "FA.h"
void run_regex_test(const string& regex) {
    cout << "══════════════════════════════" << endl;
    cout << "Testing Regex: " << regex << endl;
    
    DFA_class dfa(regex);
    dfa.print_transition_matrix();
    
    cout << "══════════════════════════════" << endl << endl;
}

int main() {
    // 测试用例列表
    vector<string> test_cases = {
        "a",         // 单字符
        "a|b",       // 选择
        "a*",        // 闭包
        "a.b",       // 显式连接（实际应为ab）
        "(a|b)*ab",   // 复杂表达式
        "a*b*cd"
    };

    // 执行所有测试
    for (const auto& re : test_cases) {
        run_regex_test(re);
    }

    return 0;
}