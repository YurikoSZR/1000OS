#ifndef EXPRESSION
#define EXPRESSION
#define EPS '#'
#include <stack>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

int arithmetic_priority(char ch) {
    switch(ch) {
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

int priority(char ch) {
    switch(ch) {
        case '*': return 3;
        case '.': return 2;
        case '|': return 1;
        default: return 0;
    }
}

string add_dot(const string& reg) {
    string buffer;
    for (size_t i = 0; i < reg.size(); ++i) {
        char current = reg[i];
        buffer.push_back(current);
        if (i + 1 < reg.size()) {
            char next = reg[i+1];
            if (current != '(' && current != '|' && current != '.' &&
                next != '*' && next != '|' && next != ')' && next != '.') {
                buffer.push_back('.');
            }
        }
    }
    return buffer;
}

string Reg_to_postfix(const string& infix) {
    string buffer;
    stack<char> stack;
    for (char ch : infix) {
        switch (ch) {
            case '(':
                stack.push(ch);
                break;
            case ')': {
                while (!stack.empty() && stack.top() != '(') {
                    buffer += stack.top();
                    stack.pop();
                }
                if (!stack.empty()) stack.pop();
                break;
            }
            case '*':
                buffer += ch;
                break;
            case '.':
            case '|': {
                while (!stack.empty() && priority(stack.top()) > priority(ch)) {
                    buffer += stack.top();
                    stack.pop();
                }
                stack.push(ch);
                break;
            }
            default:
                buffer += ch;
                break;
        }
    }
    while (!stack.empty()) {
        buffer += stack.top();
        stack.pop();
    }
    for(int i=0; i<buffer.size(); ++i)
        std::cout << buffer[i];
    std::cout << std::endl;
    return buffer;
}

string arith_to_postfix(const string& infix) {
    string buffer;
    stack<char> stack;
    for (char ch : infix) {
        switch (ch) {
            case '(':
                stack.push(ch);
                break;
            case ')': {
                while (!stack.empty() && stack.top() != '(') {
                    buffer += stack.top();
                    stack.pop();
                }
                if (!stack.empty()) stack.pop();
                break;
            }
            case '+':
            case '-':
            case '*':
            case '/':
            case '%': {
                while (!stack.empty() && arithmetic_priority(stack.top()) >= arithmetic_priority(ch)) {
                    buffer += stack.top();
                    stack.pop();
                }
                stack.push(ch);
                break;
            }
            default:
                buffer += ch;
                break;
        }
    }
    while (!stack.empty()) {
        buffer += stack.top();
        stack.pop();
    }
    return buffer;
}

struct edge {
    char ch;
    int next;
};

struct state_node {
    int count_edge = 0;
    edge edge_list[4];
    int input = 0;
    int next_state(int i) { return edge_list[i].next; }
    char transition_char(int i) { return edge_list[i].ch; }
};

struct NFA {
    int start = 0;
    int end = 0;
    int ptr = 0;
    state_node STATE[64];
    int sp_NFA = -1;
    int index[32];
    int sp = -1;
};

void add_edge(NFA& nfa, int src, int dst, char ch) {
    nfa.STATE[src].edge_list[nfa.STATE[src].count_edge++] = {ch, dst};
    nfa.STATE[dst].input++;
}

int new_node(NFA& nfa) {
    return ++nfa.sp_NFA;
}

void concatenate(NFA& nfa) {
    int end = nfa.index[nfa.sp--];
    int node_1 = nfa.index[nfa.sp--];
    int node_2 = nfa.index[nfa.sp--];
    int start = nfa.index[nfa.sp--];
    add_edge(nfa, node_2, node_1, EPS);
    nfa.index[++nfa.sp] = start;
    nfa.index[++nfa.sp] = end;
}

void union_(NFA& nfa) {
    int node_2 = nfa.index[nfa.sp--];
    int node_1 = nfa.index[nfa.sp--];
    int node_4 = nfa.index[nfa.sp--];
    int node_3 = nfa.index[nfa.sp--];
    int start = new_node(nfa);
    int end = new_node(nfa);
    add_edge(nfa, start, node_1, EPS);
    add_edge(nfa, node_2, end, EPS);
    add_edge(nfa, start, node_3, EPS);
    add_edge(nfa, node_4, end, EPS);
    nfa.index[++nfa.sp] = start;
    nfa.index[++nfa.sp] = end;
}

void union__(NFA& nfa){
    int node_2 = nfa.index[nfa.sp--];
    int node_1 = nfa.index[nfa.sp--];
    int node_4 = nfa.index[nfa.sp--];
    int node_3 = nfa.index[nfa.sp--];
    int NEW=new_node(nfa);
    add_edge(nfa,node_1,node_3,EPS);
    add_edge(nfa,node_2,NEW,EPS); 
    add_edge(nfa,node_4,NEW,EPS);  
    nfa.index[++nfa.sp] = node_1;
    nfa.index[++nfa.sp] = NEW;
}

void kleene_star(NFA& nfa) {
    int end = nfa.index[nfa.sp--];
    int start = nfa.index[nfa.sp--];
    add_edge(nfa, end, start, EPS);
    int newnode = new_node(nfa);
    add_edge(nfa, newnode, start, EPS);
    nfa.index[++nfa.sp] = newnode;
    nfa.index[++nfa.sp] = end;
}

NFA Reg2NFA(const string& reg) {
    NFA nfa;
    new_node(nfa);
    nfa.start =nfa.end= 0;
    nfa.index[++nfa.sp] = nfa.start;
    nfa.index[++nfa.sp] = nfa.end;

    for (int i =0 ; i < reg.size(); ++i) {
        char ch = reg[i];
        switch (ch) {
            case '*':
                kleene_star(nfa);
                break;
            case '|':
                union__(nfa); //union_(nfa);
                break;
            case '.':
                concatenate(nfa);
                break;
            default: {
                //获取 上一个结束状态
              
                if(reg[i+1]=='|'){
                    int end = nfa.index[nfa.sp--];
                    int START = nfa.index[nfa.sp--];
                    int node = new_node(nfa);
                    add_edge(nfa,START,node,ch);
                    int TERMINAL=new_node(nfa);
                    add_edge(nfa,node,TERMINAL,EPS);
                    add_edge(nfa,end,TERMINAL,EPS);
                    nfa.index[++nfa.sp] = START;
                    nfa.index[++nfa.sp] = TERMINAL;
                    i++;
                }
                else{
                nfa.ptr = nfa.index[nfa.sp];
                int node = new_node(nfa);
                add_edge(nfa, nfa.ptr, node, ch);
                nfa.index[++nfa.sp] = nfa.ptr;
                nfa.index[++nfa.sp] = node;
                break;
                }
            }
        }
    }

    nfa.end = nfa.index[nfa.sp];
    nfa.sp = -1;
    for(int i=0; i<=nfa.sp_NFA; i++) {
        if(nfa.STATE[i].input == 0) {
            nfa.start = i;
            break;
        }
    }
    return nfa;
}

int get_terminal(const NFA& nfa){
    for(int i=0; i<=nfa.sp_NFA; i++) {
        if(nfa.STATE[i].count_edge == 0) {
            return i;
        }
    }
    return -1;
}
void get_closure_(const NFA& nfa, int start, set<int>& closure, set<int>& visited) {
    if (visited.count(start)) return;
    visited.insert(start);
    closure.insert(start);
    for (int i = 0; i < nfa.STATE[start].count_edge; ++i) {
        if (nfa.STATE[start].edge_list[i].ch == EPS) {
            int next = nfa.STATE[start].edge_list[i].next;
            get_closure_(nfa, next, closure, visited);
        }
    }
}

set<int> set_closure(const NFA& nfa, const set<int>& states) {
    set<int> closure;
    for (int s : states) {
        set<int> visited;
        get_closure_(nfa, s, closure, visited);
    }
    return closure;
}

set<int> transition(const NFA& nfa, const set<int>& states, char ch) {
    set<int> result;
    for (int s : states) {
        for (int i = 0; i < nfa.STATE[s].count_edge; ++i) {
            if (nfa.STATE[s].edge_list[i].ch == ch) {
                result.insert(nfa.STATE[s].edge_list[i].next);
            }
        }
    }
    return result;
}

set<int> transition_closure(const NFA& nfa, const set<int>& states, char ch) {
    set<int> t = transition(nfa, states, ch);
    return set_closure(nfa, t);
}

void print_nfa_states(NFA& nfa) {
    for (int i = 0; i <= nfa.sp_NFA; ++i) {
        std::cout << "State " << i << ":\n";
        for (int j = 0; j < nfa.STATE[i].count_edge; ++j) {
            std::cout << "  Transition on '" << nfa.STATE[i].transition_char(j) 
                      << "' to State " << nfa.STATE[i].next_state(j) << "\n";
        }
    }
}
set<set<int>> generateDFAStates(const string& re) {
    // Step 1: Convert RE to NFA with new string-based methods
    string dotted = add_dot(re);
    string postfix = Reg_to_postfix(dotted);
    NFA nfa = Reg2NFA(postfix);
    print_nfa_states(nfa);
    // Step 2: Create character table from processed regex
    set<char> char_table;
    for (char ch : postfix) {
        if (ch != '*' && ch != '|' && ch != '.' && ch != EPS) {
            char_table.insert(ch);
        }
    }

    // Step 3: Compute epsilon closure of initial state
    set<int> initial_state;
    set<int> visited;
    get_closure_(nfa, nfa.start, initial_state, visited);

    // Step 4: Initialize DFA states
    set<set<int>> dfa_states;
    set<set<int>> worklist;
    dfa_states.insert(initial_state);
    worklist.insert(initial_state);

    // Step 5: Process worklist
    while (!worklist.empty()) {
        auto it = worklist.begin();
        set<int> current = *it;
        worklist.erase(it);

        for (char ch : char_table) {
            set<int> t = transition_closure(nfa, current, ch);
            if (!t.empty() && !dfa_states.count(t)) {
                dfa_states.insert(t);
                worklist.insert(t);
            }
        }
    }

    return dfa_states;
}

set<char> get_charset(const string& re) {
    set<char> charset;
    for (char ch : re) {
        if (ch != '*' && ch != '|' && ch != '.' && ch != EPS) {
            charset.insert(ch);
        }
    }
    return charset;
}

string set_to_string(const set<int>& s) {
    string ret = "{";
    for (auto it = s.begin(); it != s.end();) {
        ret += to_string(*it);
        if (++it != s.end()) ret += ",";
    }
    return ret + "}";
}

struct DFA_class {
    set<char> charset;
    set<set<int>> STATE;
    vector<vector<int>> transition_matrix;
    NFA nfa; // 保存用于生成DFA的NFA
    std::vector<std::set<int>> state_list;  // 状态集合列表
    set<int> initial_state;
    int index_of_initial_state = -1;
    set<set<int>> acceptable_states;
    DFA_class(const string& re) {
        string dotted = add_dot(re);
        string postfix = Reg_to_postfix(dotted);
        
        // 构建NFA并打印
        nfa = Reg2NFA(postfix);
        cout << "\n=== NFA States ===";
        print_nfa_states(nfa);
        int accept=get_terminal(nfa);

        // 生成字符集和DFA状态
        charset = get_charset_from_postfix(postfix);
        STATE = generateDFAStates(nfa, postfix);

        // 打印DFA状态集合
        cout << "\n=== DFA States ===";
        int state_counter = 0;
        for (const auto& s : STATE) {
            cout << "\nState " << state_counter++ << ": " 
                 << set_to_string(s);
            if (s.count(nfa.end)) cout << " [ACCEPT]";
        }

        // 构建转移矩阵
        build_transition_matrix();
        cout<<endl;
        std::cout<<"Inital State:"<<index_of_initial_state<<endl;
       for(auto& s:STATE){
          if(s.count(accept))
          acceptable_states.insert(s);
       }
    }

    void print_transition_matrix() {
        vector<char> char_list(charset.begin(), charset.end());
        // set默认有序，无需排序
        
        // 打印字符与列索引对应关系
        cout << "\nCharacter Mapping:" << endl;
        for (int j = 0; j < char_list.size(); ++j)
            cout << "  " << char_list[j] << " -> Column " << j << endl;

        // 打印转移矩阵
        cout << "\nTransition Matrix:" << endl;
        for (int i = 0; i < transition_matrix.size(); ++i) {
            cout << "State " << i << ": [";
            for (int j = 0; j < transition_matrix[i].size(); ++j) {
                cout << transition_matrix[i][j];
                if (j != transition_matrix[i].size()-1) cout << ", ";
            }
            cout << "]" << endl;
        }
    }
private:
static void print_nfa_states(const NFA& nfa) {
    cout << "\nNFA has " << (nfa.sp_NFA+1) << " states:";
    for (int i = 0; i <= nfa.sp_NFA; ++i) {
        cout << "\n  State " << i << " (";
        if (i == nfa.start) cout << "START ";
        if (i == nfa.end) cout << "END";
        cout << ")";
        
        for (int j = 0; j < nfa.STATE[i].count_edge; ++j) {
            cout << "\n    ─[" << nfa.STATE[i].edge_list[j].ch 
                 << "]─▶ " << nfa.STATE[i].edge_list[j].next;
        }
    }
    cout << "\n" << endl;
}
    set<char> get_charset_from_postfix(const string& postfix) {
        set<char> charset;
        for (char ch : postfix) {
            if (ch != '*' && ch != '|' && ch != '.' && ch != EPS) {
                charset.insert(ch);
            }
        }
        return charset;
    }

    set<set<int>> generateDFAStates(const NFA& nfa, const string& postfix) {
        set<char> char_table = get_charset_from_postfix(postfix);

        set<int> initial;
        set<int> visited_init;
        get_closure_(nfa, nfa.start, initial, visited_init);

        set<set<int>> dfa_states;
        set<set<int>> worklist;
        this->initial_state = initial;
        dfa_states.insert(initial);
        worklist.insert(initial);

        while (!worklist.empty()) {
            auto current = *worklist.begin();
            worklist.erase(worklist.begin());

            for (char ch : char_table) {
                set<int> t = transition_closure(nfa, current, ch);
                if (t.empty()) continue;

                if (!dfa_states.count(t)) {
                    dfa_states.insert(t);
                    worklist.insert(t);
                }
            }
        }

        return dfa_states;
    }

    void build_transition_matrix() {
        vector<set<int>> state_list(STATE.begin(), STATE.end());
        map<set<int>, int> state_index;
        for (int i = 0; i < state_list.size(); ++i) {
            state_index[state_list[i]] = i;
        }

        vector<char> char_list(charset.begin(), charset.end());
        transition_matrix.resize(state_list.size(), vector<int>(char_list.size(), -1));

        for (int i = 0; i < state_list.size(); ++i) {
            const set<int>& s = state_list[i];
            for (int j = 0; j < char_list.size(); ++j) {
                char c = char_list[j];
                set<int> t = transition_closure(nfa, s, c);
                if (!t.empty() && state_index.count(t)) {
                    transition_matrix[i][j] = state_index.at(t);
                }
            }
        }
        for(auto &s: state_list){
            if( s == initial_state){
            index_of_initial_state = state_index.at(s);
            break;
            }
        }
    }
};


void run_regex_test(const string& regex) {
    cout << "══════════════════════════════" << endl;
    cout << "Testing Regex: " << regex << endl;
    
    DFA_class dfa(regex);
    dfa.print_transition_matrix();
    
    cout << "══════════════════════════════" << endl << endl;
}
#endif