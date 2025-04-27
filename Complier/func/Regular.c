#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void modify_regx(char * reg,char* buffer){
    int ptr = 0;
    while(*reg){
        switch(*reg){
            case '|':
                buffer[ptr++] = '|';
                reg++;
                break;
            case '*':
                buffer[ptr++] = '*';
                reg++;
                break;
            case '\\':
                switch( *(reg+1) ){
                    case '*':
                        buffer[ptr++]=*(reg+1);
                        break;
                    case '|':
                        buffer[ptr++]=*(reg+1);
                        break;
                    case '+':
                        buffer[ptr++]=*(reg+1);
                        break;
                }
                reg = reg+2;
                break;
            default:
                if( *(reg+1)!='*' && *(reg+1)!='|' && *(reg+1)!='+' && *(reg+1)!='|' ){
                    buffer[ptr++] = *reg;
                    buffer[ptr++] = '+';
                }
                else{
                    buffer[ptr++] = *reg;
                }
                reg++;
                break;
        }        
    }
}

int  main(){
    char buffer[32];
    char test[]="a|bcd*|ef";
    modify_regx(test,buffer);
    printf("%s\n",buffer);
}