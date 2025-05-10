#include "Automaton.h"
int main() {
    // 测试用例列表
    vector<string> test_cases = {
        "if",
        "else",
        "while",
        "for",
        "int",
        "char",
        "a|b",
    };

    const string str="for(int a)";
    for(auto c:test_cases){
        DFA_class TEST(c);
        cout<<TEST.pattern(str)<<endl;
    }



    DFA_class test(test_cases[0]);
    //test.pattern("0xf1111");
    test.print_transition_matrix();
    cout << "══════════════════════════════" << endl << endl;
    const string re = "0xbdce110111";
    bool result= test.pattern(re);
    std::cout<<result<<endl;
    return 0;
}