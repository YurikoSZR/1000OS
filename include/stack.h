#include <iostream>
#define MAX 256
template<typename T>
class stack{
    public:
    T table[MAX];
    int sp=-1;
    void push(T obj){
        if(sp== 255){
            std::cout<<"Stack Overflow"<<std::endl;
            return;
        }
        table[++sp]=obj;
    }
    void pop(T& di){
        if(sp==-1){
            std::cout<<"Stack is empty,Warning"<<std::endl;
            return;
        }
        di=table[sp--];
    }

    int& top(){
        return table[sp];
    }
};