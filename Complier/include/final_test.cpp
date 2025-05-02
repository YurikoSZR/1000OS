#include "Automaton.h"
int main() {
    // 测试用例列表
    vector<string> test_cases = {
        "0|(1(0|1)*)"
    };

    // // 执行所有测试
    // for (const auto& re : test_cases) {
    //     run_regex_test(re);
    // }
    DFA_class test(test_cases[0]);
    //test.pattern("0xf1111");
    test.print_transition_matrix();
    cout << "══════════════════════════════" << endl << endl;
    const string re = "1000100";
    bool result= test.pattern(re);
    std::cout<<result<<endl;
    return 0;
}