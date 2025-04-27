#include <stdio.h>
#include "expression.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>

struct State {
    std::unordered_map<char, std::vector<State*>> transitions;
    bool is_accept = false;
};

struct NFA {
    State* start;
    State* accept;
};

State* create_state(bool is_accept = false) {
    return new State{std::unordered_map<char, std::vector<State*>>(), is_accept};
}

NFA* create_nfa(State* start, State* accept) {
    return new NFA{start, accept};
}

NFA* create_nfa_from_char(char ch) {
    State* start = create_state();
    State* accept = create_state(true);
    start->transitions[ch].push_back(accept);
    return create_nfa(start, accept);
}

NFA* concatenate(NFA* nfa1, NFA* nfa2) {
    nfa1->accept->transitions['\0'].push_back(nfa2->start);
    nfa1->accept->is_accept = false;
    return create_nfa(nfa1->start, nfa2->accept);
}

NFA* or_nfa(NFA* nfa1, NFA* nfa2) {
    State* start = create_state();
    State* accept = create_state(true);

    start->transitions['\0'].push_back(nfa1->start);
    start->transitions['\0'].push_back(nfa2->start);

    nfa1->accept->transitions['\0'].push_back(accept);
    nfa2->accept->transitions['\0'].push_back(accept);

    nfa1->accept->is_accept = false;
    nfa2->accept->is_accept = false;

    return create_nfa(start, accept);
}

NFA* star_nfa(NFA* nfa) {
    State* start = create_state();
    State* accept = create_state(true);

    start->transitions['\0'].push_back(nfa->start);
    start->transitions['\0'].push_back(accept);

    nfa->accept->transitions['\0'].push_back(nfa->start);
    nfa->accept->transitions['\0'].push_back(accept);

    nfa->accept->is_accept = false;

    return create_nfa(start, accept);
}

NFA* postfix_to_nfa(const std::string& postfix) {
    std::stack<NFA*> stack;

    for (char ch : postfix) {
        switch (ch) {
            case '.':
                {
                    NFA* nfa2 = stack.top(); stack.pop();
                    NFA* nfa1 = stack.top(); stack.pop();
                    stack.push(concatenate(nfa1, nfa2));
                }
                break;
            case '|':
                {
                    NFA* nfa2 = stack.top(); stack.pop();
                    NFA* nfa1 = stack.top(); stack.pop();
                    stack.push(or_nfa(nfa1, nfa2));
                }
                break;
            case '*':
                {
                    NFA* nfa = stack.top(); stack.pop();
                    stack.push(star_nfa(nfa));
                }
                break;
            default:
                stack.push(create_nfa_from_char(ch));
                break;
        }
    }

    return stack.top();
}

void print_nfa(NFA* nfa) {
    std::stack<State*> state_stack;
    std::unordered_map<State*, int> state_ids;
    int id_counter = 0;

    state_stack.push(nfa->start);
    state_ids[nfa->start] = id_counter++;

    while (!state_stack.empty()) {
        State* current = state_stack.top();
        state_stack.pop();

        std::cout << "State " << state_ids[current] << (current->is_accept ? " (accept)" : "") << ":\n";

        for (const auto& [symbol, targets] : current->transitions) {
            for (State* target : targets) {
                if (state_ids.find(target) == state_ids.end()) {
                    state_ids[target] = id_counter++;
                    state_stack.push(target);
                }
                std::cout << "  On '" << (symbol == '\0' ? 'ε' : symbol) << "' go to State " << state_ids[target] << "\n";
            }
        }
    }
}

int main() {
    char* regex = "a.b|c*";
    char postfix[100];
    clean_buffer(postfix);
    add_dot(regex, postfix);
    Reg_to_postfix(postfix, postfix);

    std::cout << "Postfix: " << postfix << "\n";

    NFA* nfa = postfix_to_nfa(postfix);
    print_nfa(nfa);

    return 0;
}