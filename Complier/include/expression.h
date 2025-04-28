#ifndef EXPRESSION
#define EXPRESSION
#define EPS '#'
#include <set>
#include <vector>
using namespace std;
struct stack_c{
    char data[32];
    int sp=-1;
    char top(){return data[sp];}
    char pop(){
        if(sp<0)return 0;
        return data[sp--];
    }
    void push(char ch){
        if(sp<32)data[++sp]=ch;
    }
};
void clean_buffer(char* buffer);
int arithmetic_priority(char ch){
    switch(ch){
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
int priority(char ch){
    switch(ch){
        case '*': return 3;
        case '.': return 2;
        case '|': return 1;
        default: return 0;
    }
}

void add_dot(char* reg,char* buffer){
    int sp=0;
    A:
    while(*reg){
        switch(*reg){
            case '*':
            case '|':
            case '(':
            case ')':
                buffer[sp++]=*reg;
                reg++;break;
            default:
                buffer[sp++]=*reg;
                if(*(reg+1)!='*' && *(reg+1)!='|' && *(reg+1)!=')' && *(reg+1)!=0)
                    buffer[sp++]='.';
                reg++;
        }
    }
}

void Reg_to_postfix(char* infix,char* buffer){
    int ptr = 0;
    int sp=0;
    stack_c stack;
    while(infix[ptr]){
        switch(infix[ptr]){
            case '(':
                stack.push(infix[ptr++]);
                break;
            case ')':
                ptr++;
                while(stack.top()!='(' && stack.sp>=0)buffer[sp++]=stack.pop();
                stack.pop();
                break;
            case '*':
                if(stack.top()=='*')break;
                buffer[sp++]=infix[ptr++];
                break;
            case '.':
            case '|':
            if(stack.sp>=0 ){
                if(priority(stack.top())>=priority(infix[ptr])){
                    buffer[sp++]=stack.pop();
                    stack.push(infix[ptr++]);
                }
                else{
                    stack.push(infix[ptr++]);
                }
            }
            else{
                stack.push(infix[ptr++]);
            }
            break;

            default:
                buffer[sp++]=infix[ptr++];
                break;
        }
    }
    while(stack.sp>=0)
        buffer[sp++]=stack.pop();
}

void arith_to_postfix(char* infix,char* buffer){
    int ptr = 0;
    int sp=0;
    stack_c stack;
    while(infix[ptr]){
        switch(infix[ptr]){
            case '(':
                stack.push(infix[ptr++]);
                break;
            case ')':
                ptr++;
                while(stack.top()!='(' && stack.sp>=0)buffer[sp++]=stack.pop();
                stack.pop();
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            if(stack.sp>=0 ){
                if(priority(stack.top())>=priority(infix[ptr])){
                    buffer[sp++]=stack.pop();
                    stack.push(infix[ptr++]);
                }
                else{
                    stack.push(infix[ptr++]);
                }
            }
            else{
                stack.push(infix[ptr++]);
            }
            break;

            default:
                buffer[sp++]=infix[ptr++];
                break;
        }
    }
    while(stack.sp>=0)
        buffer[sp++]=stack.pop();
}

void operation(char* reg,char* buffer){
    char dot[128];
    clean_buffer(dot);
    add_dot(reg,dot);
    Reg_to_postfix(dot,buffer);
}

void clean_buffer(char* buffer){
    for(int i=0;i<32;i++)buffer[i]=0;
}
struct edge {
    char ch;
    int next;
};

struct state_node {
    int count_edge = 0;
    edge edge_list[3];
    int input = 0;
    int next_state(int i){
        return edge_list[i].next;
    }
    char transition_char(int i){
        return edge_list[i].ch;
    }
};

struct NFA {
    int start = 0;
    int end = 0;
    int ptr = 0;
    state_node STATE[64];  // Added state_node array
    int sp_NFA = -1;      // Added stack pointer for state_node array
    int index[32];       // Added index stack
    int sp = -1;          // Added stack pointer for index stack
};

void add_edge(NFA* nfa, int src, int dst, char ch) {
    nfa->STATE[src].edge_list[nfa->STATE[src].count_edge].ch = ch;
    nfa->STATE[src].edge_list[nfa->STATE[src].count_edge].next = dst;
    nfa->STATE[src].count_edge++;
    nfa->STATE[dst].input += 1;
}

void delete_edge(NFA* nfa, int dst) {
    nfa->STATE[dst].edge_list[nfa->STATE[dst].count_edge].ch = EPS;
    nfa->STATE[dst].edge_list[nfa->STATE[dst].count_edge].next = 0;
    nfa->STATE[dst].count_edge--;
}

int new_node(NFA* nfa) {
    return ++nfa->sp_NFA;
}

void concatenate(NFA* nfa) {
    int end = nfa->index[nfa->sp--];
    int node_1 = nfa->index[nfa->sp--];
    int node_2 = nfa->index[nfa->sp--];
    int start = nfa->index[nfa->sp--];
    add_edge(nfa, node_2, node_1, EPS);
    nfa->index[++nfa->sp] = start;
    nfa->index[++nfa->sp] = end;
}

void union_(NFA* nfa) {
    int node_2 = nfa->index[nfa->sp--];
    int node_1 = nfa->index[nfa->sp--];
    int node_4 = nfa->index[nfa->sp--];
    int node_3 = nfa->index[nfa->sp--];
    int start = new_node(nfa);
    int end = new_node(nfa);
    add_edge(nfa, start, node_1, EPS);
    add_edge(nfa, node_2, end, EPS);
    add_edge(nfa, start, node_3, EPS);
    add_edge(nfa, node_4, end, EPS);
    nfa->index[++nfa->sp] = start;
    nfa->index[++nfa->sp] = end;
}

void kleene_star(NFA* nfa) {
    int node_1 = new_node(nfa);
    int node_2 = new_node(nfa);
    int end = nfa->index[nfa->sp--];
    int start = nfa->index[nfa->sp--];
    add_edge(nfa, node_1, start, EPS);
    add_edge(nfa, end, node_2, EPS);
    add_edge(nfa, end, start, EPS);
    nfa->index[++nfa->sp] = node_1;
    nfa->index[++nfa->sp] = node_2;
}

NFA Reg2NFA(char* reg) {
    NFA nfa;
    new_node(&nfa);
    nfa.start = nfa.end = 0;
    nfa.index[++(nfa.sp)] = nfa.start;
    nfa.index[++(nfa.sp)] = nfa.end;
    while (*reg) {
        switch (*reg) {
            case '*':
                kleene_star(&nfa);
                reg++;
                break;
            case '|':
                union_(&nfa);
                reg++;
                break;
            case '.':
                concatenate(&nfa);
                reg++;
                break;
            default:
                nfa.ptr = nfa.index[nfa.sp];
                int node = new_node(&nfa);
                add_edge(&nfa, nfa.ptr, node, *reg);
                nfa.index[++(nfa.sp)] = nfa.ptr;
                nfa.index[++(nfa.sp)] = node;
                reg++;
        }
        nfa.end = nfa.index[nfa.sp];
    }
    nfa.sp = -1;
    for(int i=0;i<nfa.sp_NFA;i++){
        if(nfa.STATE[i].input==0){
            nfa.start=i;
            break;
        }
    }
    return nfa;
}

struct DFA{
    int valid;
    vector<edge> transition;
    vector<set<int>> STATE;
};
void get_closure_(NFA* nfa,int start,set<int>& closure,set<int>& complete){
    closure.insert(start);
    for(int i=0;i<3;i++){
        if(nfa->STATE[start].edge_list[i].ch==EPS){
            closure.insert(nfa->STATE[start].edge_list[i].next);
            auto exist=complete.count(nfa->STATE[start].edge_list[i].next);
            if(exist== 0){
                complete.insert(nfa->STATE[start].edge_list[i].next);
                get_closure_(nfa,nfa->STATE[start].edge_list[i].next,closure,complete);
            }
        }
    }
}
set<int> transition(NFA& nfa,set<int>& state,char ch){
    set<int> result;
    for(auto s:state){
        for(int i=0;i<3;i++)
        if(nfa.STATE[s].edge_list[i].ch==ch){
            result.insert(nfa.STATE[s].edge_list[i].next);
        }
    }
    return result;
}

set<int> set_closure(NFA& nfa,set<int>& state){
    set<int> result;
    set<int> complete;
    for (int t : state){
        get_closure_(&nfa,t,result,complete);
    }
    return result;
}

set<int> transition_closure(NFA& nfa,set<int>& state,char ch){
    set<int> transit=transition(nfa,state,ch);
    return set_closure(nfa,transit);
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

#endif