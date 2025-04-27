#include<stdio.h>
#include <ctype.h> // For isdigit and tolower

int hex_to_decimal(char *str) {
    int decimal = 0;
    while(*str){
        if( (*str)>='0' && (*str)<='9'){
            decimal = decimal*16 + (*str-'0');
            str++;
            continue;
        }
        if( (*str)>='a' && (*str)<='f'){
            decimal = decimal*16 + ((*str-'a')+10);
            str++;
            continue;
        }
        else{
            printf("Invalid hexadecimal digit: %c\n", *str);
            return -1;
        }
    }
    return decimal;
}


struct expr{
    int start,end;
};

int atoi(char* str){
    int num = 0;
    while(str[0] != '\0'){
        num = num*10 + (str[0]-'0');
        str++;
    }
    return num;
}

struct expr fetch_bracket(char *str,int  start){
    int depth = -1;
    struct expr E;
    for(int i = start; str[i] != '\0'; i++){
        if(depth==-1 &&str[i] == '('){
            depth=1;
            E.start = i;
            continue;
        }
        if( str[i] == '('){
            depth++;
            continue;
        }
        if( str[i] == ')'){
            depth--;
        }
        if( depth == 0){
            E.end = i;
            return E;
        }
    }
    printf("Brackets don't match\n");
    E.start = -1;
    E.end = -1;
    return E;
}

int fetch_number(char* str,int start){
    char buffer[32];
    int ptr = 0;
    if( str[start] == '0' && str[start+1] == 'x')goto hex;
    decimal:
    while(str[start]>='0' && str[start]<='9'){
        buffer[ptr++] = str[start++];
    }
    return atoi(buffer);
    hex:
    start+=2;
    while((str[start]>='0' && str[start]<='9') || (str[start]>='a' && str[start]<='f')){
        buffer[ptr++] = str[start++];
    }
    return hex_to_decimal(buffer);
}

int main(){
  char str[] = "0xffff";
  printf("0xffff=%d\n",fetch_number(str,0));
}