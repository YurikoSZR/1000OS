#ifndef EXPRESSION
#define EXPRESSION
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

int arithmetic_priority(char ch){
    switch(ch){
        case '*':
        case '/':
        case '%':
            return 2;
            break;
        case '+':
        case '-':
            return 1;
            break;
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
                buffer[sp++]=infix[ptr++];
                break;
            case '.':
            case '|':
            if(stack.sp>=0 ){
                if(priority(stack.top()) >= priority(infix[ptr])){
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
    int ptr = 0;int sp=0;
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
                if(arithmetic_priority(stack.top())>=arithmetic_priority(infix[ptr])){
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


void clean_buffer(char* buffer){
    for(int i=0;i<32;i++)buffer[i]=0;
}

void add_dot(char* reg,char* buffer){
    int sp=0;
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
                if(*(reg+1)!='*' && *(reg+1)!='|' &&*(reg+1)!=')')
                    buffer[sp++]='.';
                reg++;
        }
    }
}
#endif