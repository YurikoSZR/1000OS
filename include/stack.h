#include <iostream>
#define MAX 512
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

template<typename T>
class queue{
    public:
    T list[MAX];
    int head=0,count=0;
    
    void push(T obj){
        if(count==MAX){
            std::cout<<"Queue Overflow"<<std::endl;
            return;
        }
        count++;
        list[(head+count-1)%MAX]=obj;        
    }
    int pop(){
        if(count==0){
            std::cout<<"Queue Underflow"<<std::endl;
            return 0x8fffffff;
        }
        count--;
        int result=list[head];
        head=(head+1)%MAX;
    }
    int& front(){
        return list[head];
    }
};


