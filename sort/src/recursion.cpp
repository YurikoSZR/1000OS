#include <stack.h>

void hanoi(int n,int from,int via,int to){
    static stack<int> local_stack;
    int state;
    local_stack.sp+=5;
    local_stack.table[local_stack.sp]=0;
    local_stack.table[local_stack.sp-1]=n;
    local_stack.table[local_stack.sp-2]=from;
    local_stack.table[local_stack.sp-3]=via;
    local_stack.table[local_stack.sp-4]=to;
    while(local_stack.sp>=0){
        switch(local_stack.table[local_stack.sp]){
            case 0:
            if(local_stack.table[local_stack.sp-1]==1){
                std::cout<<local_stack.table[local_stack.sp-2]<<"----->"<<local_stack.table[local_stack.sp-4]<<std::endl;
                local_stack.sp-=5;        
            }
            else{
                local_stack.table[local_stack.sp]+=1;
                state=local_stack.table[local_stack.sp-1];
                from=local_stack.table[local_stack.sp-2];
                via=local_stack.table[local_stack.sp-3];
                to=local_stack.table[local_stack.sp-4];
                local_stack.sp+=5;
                local_stack.table[local_stack.sp]=0;
                local_stack.table[local_stack.sp-1]=state-1;
                local_stack.table[local_stack.sp-2]=from;
                local_stack.table[local_stack.sp-3]=to;
                local_stack.table[local_stack.sp-4]=via;
            }
            break;
            case 1:
                local_stack.table[local_stack.sp]+=1;
                from=local_stack.table[local_stack.sp-2];
                via=local_stack.table[local_stack.sp-3];
                to=local_stack.table[local_stack.sp-4];
                local_stack.sp+=5;
                local_stack.table[local_stack.sp]=0;
                local_stack.table[local_stack.sp-1]=1;
                local_stack.table[local_stack.sp-2]=from;
                local_stack.table[local_stack.sp-3]=via;
                local_stack.table[local_stack.sp-4]=to;
                break;
            case 2:
                local_stack.table[local_stack.sp]+=1;
                state=local_stack.table[local_stack.sp-1];
                from=local_stack.table[local_stack.sp-2];
                via=local_stack.table[local_stack.sp-3];
                to=local_stack.table[local_stack.sp-4];
                local_stack.sp+=5;
                local_stack.table[local_stack.sp]=0;
                local_stack.table[local_stack.sp-1]=state-1;
                local_stack.table[local_stack.sp-2]=via;
                local_stack.table[local_stack.sp-3]=from;
                local_stack.table[local_stack.sp-4]=to;
                break;
            case 3:
                local_stack.sp-=5;
        }
    }
}



int main(){
    hanoi(3,11,22,33);
}