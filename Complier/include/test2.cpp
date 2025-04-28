#include <stdio.h>
#include <stdlib.h>
#include "expression.h"
#include <iostream>
int main() {
    char array[] = "a*b|c";
    char buffer[512];
    operation(array, buffer);
    NFA nfa = Reg2NFA(buffer);
    set<int> closure;
    set<int> complete;
    get_closure_(EPS,&nfa,6,closure,complete);
    for(auto item=closure.begin();item!=closure.end();++item){
		std::cout<<*item<<std::endl;
	}
}