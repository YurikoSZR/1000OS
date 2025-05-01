#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include "expression.h"
using namespace std;

// Custom hash function for std::set<int>
struct SetHash {
    size_t operator()(const set<int>& s) const {
        size_t hash_value = 0;
        for (const int& elem : s) {
            hash_value ^= hash<int>()(elem); // XOR hashes of elements
        }
        return hash_value;
    }
};

// Function to generate DFA states from a given regular expression
DFA generateDFA(char* re) {
    // Step 1: Convert RE to NFA
    char buffer[256];
    clean_buffer(buffer);
    operation(re, buffer);
    NFA nfa = Reg2NFA(re);

    // Print the intermediate NFA
    print_nfa_states(nfa);

    // Step 2: Create a character table excluding '*', '|', and '()'
    set<char> char_table;
    for (const char* p = re; *p; ++p) {
        if (*p != '*' && *p != '|' && *p != '(' && *p != ')') {
            char_table.insert(*p);
        }
    }

    // Step 3: Compute the epsilon closure of the initial state
    set<int> initial_state;
    set<int> complete;
    get_closure_(nfa, nfa.start, initial_state, complete);

    // Step 4: Initialize DFA states and worklist
    set<set<int>> dfa_states;
    set<set<int>> worklist;
    dfa_states.insert(initial_state);
    worklist.insert(initial_state);

    // Step 5: Process worklist to generate DFA states
    unordered_map<set<int>, int, SetHash> state_map; // Use custom hash
    int state_index = 0;
    state_map[initial_state] = state_index++;

    while (!worklist.empty()) {
        set<int> current_state = *worklist.begin();
        worklist.erase(worklist.begin());

        for (char ch : char_table) {
            set<int> transition_state = transition_closure(nfa, current_state, ch);
            if (!transition_state.empty() && dfa_states.find(transition_state) == dfa_states.end()) {
                dfa_states.insert(transition_state);
                worklist.insert(transition_state);
                state_map[transition_state] = state_index++;
            }
        }
    }

    // Step 6: Construct DFA structure
    DFA dfa;
    dfa.valid = state_index;
    dfa.STATE.resize(state_index);

    for (const set<int>& state : dfa_states) {
        int current_dfa_state = state_map[state];
        for (char ch : char_table) {
            set<int> transition_state = transition_closure(nfa, state, ch);
            if (!transition_state.empty()) {
                int next_dfa_state = state_map[transition_state];
                dfa.transition.push_back({ch, next_dfa_state});
                dfa.STATE[current_dfa_state].insert(next_dfa_state);
            }
        }
    }

    return dfa;
}

int main() {
    char re[] = "a|b*";
    DFA dfa = generateDFA(re);

    // Print DFA states and transitions for verification
    cout << "DFA States and Transitions:" << endl;
    for (int i = 0; i < dfa.valid; ++i) {
        cout << "State " << i << ": { ";
        for (int next_state : dfa.STATE[i]) {
            cout << next_state << " ";
        }
        cout << "}" << endl;
    }

    cout << "\nTransitions:" << endl;
    return 0;
}