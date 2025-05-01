// #include <stdio.h>
// #include <stdlib.h>
// #include "expression.h"
// #include <iostream>
// int main() {
//     char array[] = "a*b|c";
//     char buffer[512];
//     operation(array, buffer);
//     NFA nfa = Reg2NFA(buffer);
//     set<int> closure;
//     set<int> complete;
//     get_closure_(&nfa,6,closure,complete);
//     for(auto item=closure.begin();item!=closure.end();++item){
// 		std::cout<<*item<<std::endl;
// 	}
// }
#include <iostream>
#include <set>
#include <vector>
#include "expression.h"

void process_regex(char* reg) {
  // Step 1: Construct the NFA for the given regular expression
  char buffer[512];
  operation(reg, buffer);
  NFA nfa = Reg2NFA(buffer);

  // Step 2: Calculate the initial state's epsilon closure
  std::cout << "NFA for regex: " << reg << "\n";
  print_nfa_states(nfa);
  set<int> initial_closure;
  set<int> complete;
  get_closure_(nfa, nfa.start, initial_closure, complete);

  // Step 3: Calculate the transition state for character 'a' from the initial state's epsilon closure
  set<int> transition_a = transition_closure(nfa, initial_closure, 'a');

  // Step 4: Print the initial state's epsilon closure and the transition state for 'a'
  std::cout << "Initial state's epsilon closure: ";
  for (int state : initial_closure) {
      std::cout << state << " ";
  }
  std::cout << std::endl;

  std::cout << "Transition state for 'a': ";
  for (int state : transition_a) {
      std::cout << state << " ";
  }
  std::cout << std::endl;

  std::cout << "----------------------------------------\n";
}

int main() {
  // Regular Expression 1: (a|b)*c
  char reg1[] = "(a|b)*c";
  process_regex(reg1);

  // Regular Expression 2: a*(b|c)*d
  char reg2[] = "a*(b|c)*d";
  process_regex(reg2);

  // Regular Expression 3: (a|b|c|d)*
  char reg3[] = "(a|b|c|d)*";
  process_regex(reg3);

  return 0;
}