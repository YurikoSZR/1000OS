#include "Automaton.h"
#define INT_MAX 2147483647
#include <chrono>

vector<string> token_table; // 全局token存储

// 正则表达式到优先级映射表
vector<pair<string, int>> regex_patterns = {
    {"(while)", 1},    // 关键字最高优先级
    {"(for)", 1},
    {"(if)", 1},
    {"(else)", 1},
    {"(int)", 1},
    {"(char)", 1},
    {"(0|1(0|1)*)", 2}, // 二进制数较低优先级
    {"(a|i)*",2}
};

// DFA管理器
struct DFAManager {
    vector<DFA_class> dfas;
    vector<int> priorities;
    
    DFAManager() {
        // 编译所有正则表达式
        for(auto& p : regex_patterns) {
            try {
                dfas.emplace_back(p.first);
                priorities.push_back(p.second);
            } catch(...) {
                cerr << "Error compiling regex: " << p.first << endl;
                exit(1);
            }
        }
    }

    // 带优先级的最大匹配
    pair<int, int> find_max_match(const string& s, int start_pos) {
        int max_len = -1;
        int max_priority = INT_MAX; 
        int selected_dfa = -1;

        // 初始化所有DFA状态
        vector<int> states(dfas.size(), 0);
        for(size_t i=0; i<dfas.size(); ++i) {
            states[i] = dfas[i].index_of_initial_state;
        }

        for(int pos=start_pos; pos<s.length(); ++pos) {
            bool all_dead = true;

            for(size_t dfa_idx=0; dfa_idx<dfas.size(); ++dfa_idx) {
                if(states[dfa_idx] == -1) continue;

                char c = s[pos];
                int trans = dfas[dfa_idx].translate_ch(c);
                if(trans == -1) {
                    states[dfa_idx] = -1;
                    continue;
                }

                int next_state = dfas[dfa_idx].transition_matrix[states[dfa_idx]][trans];
                states[dfa_idx] = next_state;
                
                if(next_state != -1) {
                    all_dead = false;
                    
                    // 检查是否接受状态
                    auto& accept = dfas[dfa_idx].Accept;
                    if(find(accept.begin(), accept.end(), next_state) != accept.end()) {
                        int cur_len = pos - start_pos + 1;
                        if(cur_len > max_len || (cur_len == max_len && priorities[dfa_idx] < max_priority)) {
                            max_len = cur_len;
                            max_priority = priorities[dfa_idx];
                            selected_dfa = dfa_idx;
                        }
                    }
                }
            }

            if(all_dead) break;
        }

        return {selected_dfa, max_len};
    }
};

// 分词主函数
void tokenize(const string& input) {
    static DFAManager manager;
    
    int pos = 0;
    while(pos < input.length()) {
        // 跳过空白字符
        if(isspace(input[pos])) {
            ++pos;
            continue;
        }

        auto [dfa_idx, len] = manager.find_max_match(input, pos);
        
        if(dfa_idx == -1 || len <= 0) {
            cerr << "Lexical error at position " << pos << endl;
            ++pos; // 错误恢复：跳过无法识别的字符
            continue;
        }

        string token = input.substr(pos, len);
        token_table.push_back(token);
        
        // 特殊处理关键字
        if(dfa_idx <= 5) { // 前6个是关键字
            token_table.back() = "<KEYWORD:" + token + ">";
        }

        pos += len;
    }
}
void benchmark() {
    string big_code(1e6, '0'); // 生成1MB测试数据
    auto start = chrono::high_resolution_clock::now();
    tokenize(big_code);
    auto duration = chrono::duration_cast<chrono::milliseconds>(
        chrono::high_resolution_clock::now() - start
    );
    cout << "Tokenized 1MB in " << duration.count() << "ms" << endl;
}
int main() {
    benchmark();
}
