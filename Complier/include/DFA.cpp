#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include "FA.h"
using namespace std;

// Function to generate DFA states from a given regular expression


int main() {
    std::string re = "(a*b|c)*de";
    set<set<int>> dfa_states = generateDFAStates(re);

    // Print DFA states for verification
    int state_index = 0;
    for (const set<int>& state : dfa_states) {
        cout << "DFA State " << state_index++ << ": { ";
        for (int nfa_state : state) {
            cout << nfa_state << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}
