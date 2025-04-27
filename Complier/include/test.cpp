#include <stdio.h>
#include <stdlib.h>
#include "expression.h"
#define EPS '\0'
//简化REG 转换为 NFA(不考虑字符集)
struct edge{
    char ch;
    int next;
};

struct state_node{
    int count_edge=0;
    edge edge_list[3];
    int input=0;
};

struct NFA{
    int start=0;
    int end=0;
    int ptr=0;
};

state_node STATE[64];
int sp_NFA=-1;

void add_edge(int src,int dst,char ch){
    STATE[src].edge_list[STATE[src].count_edge].ch=ch;
    STATE[src].edge_list[STATE[src].count_edge].next=dst;
    STATE[src].count_edge++;
    STATE[dst].input+=1;
}
void delete_edge(int dst){
    STATE[dst].edge_list[STATE[dst].count_edge].ch=EPS;
    STATE[dst].edge_list[STATE[dst].count_edge].next=0;
    STATE[dst].count_edge--;
}

int new_node(){
    return ++sp_NFA;
}

stack_c stack;
int index[32];int sp = -1;

void concatenate(NFA* nfa){
   int end=index[sp--];
   int node_1=index[sp--];
   int node_2=index[sp--];
   int start=index[sp--];
   add_edge(node_2,node_1,EPS);
   index[++sp]=start;
   index[++sp]=end;
}

void union_(NFA* nfa){
   int node_2=index[sp--];
   int node_1=index[sp--];
   int node_4=index[sp--];
   int node_3=index[sp--];
   int start=new_node();
   int end=new_node();
   add_edge(start,node_1,EPS);
   add_edge(node_2,end,EPS);
   add_edge(start,node_3,EPS);
   add_edge(node_4,end,EPS);
   index[++sp]=start;
   index[++sp]=end;
}

void kleene_star(NFA *nfa){
    int node_1=new_node();int node_2=new_node();
    int end = index[sp--];int start=index[sp--];
    add_edge(node_1,start,EPS);
    add_edge(end,node_2,EPS);
    add_edge(end,start,EPS);
    index[++sp]=node_1;
    index[++sp]=node_2;
}

NFA Reg2NFA(char* reg){
    NFA nfa;
    new_node();
    nfa.start=nfa.end=0;
    index[++sp]=nfa.start;
    index[++sp]=nfa.end;
    while(*reg){
        switch(*reg){
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
                nfa.ptr=index[sp];
                int node=new_node();
                add_edge(nfa.ptr,node,*reg);
                index[++sp]=nfa.ptr;
                index[++sp]=node;
                reg++;
        }
        nfa.end=index[sp];
    }
    sp=-1;
    return nfa;
}

int main(){
    char array[]="(a|b)*c";
    char buffer[32];
    operation(array,buffer);
    NFA nfa=Reg2NFA(buffer);
    int start;
    for(int i=0;i<=sp_NFA;i++){
        if(STATE[i].input==0){
            start=i;
            break;
        }
    }
    char test[]="ababc";
    for(int i=0;test[i];i++){
        if(STATE[start].count_edge==0)break;
        for(int j = 0;j<3;j++){
            if(STATE[start].edge_list[j].ch==test[j]){
                start=STATE[start].edge_list[j].next;
                break;
            }
            if(j==2){
                for(int k=0;k<3;k++)
                if(STATE[start].edge_list[k].ch==EPS){
                    start=STATE[start].edge_list[k].next;
                    break;
                }
            }
        }
    }
    if(STATE[start].input==0 && STATE[start].count_edge==0)
    printf("Invalid\n");
    else if(STATE[start].input>0 && STATE[start].count_edge==0)
    printf("Valid\n");
    else printf("Invalid\n");
}