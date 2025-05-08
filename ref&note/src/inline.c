#include <stdio.h>
__attribute__((always_inline)) //强制内联函数
inline int test(int n){
        return n*(n-1);
}

int main(){
    int n = 5;
    n=test(5);
    printf("%d\n",n);
}
