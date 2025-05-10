#pragma once
#include <unistd.h>
int atoi(char* str){
    int i=0,n=0;
    if(str[i]=='-')i++;
    while(str[i]){
        n=n*10+str[i]-'0';
        i++;
    }
    return str[0]=='-'?-n:n;
}

int length(const char* str){
    int i=0;
    while(str[i])i++;
    return i;
}


void print(char* str){
    write(1,str,length(str));
}