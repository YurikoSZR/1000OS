#include <stdio.h>

int main(){
    int a =({int val = 10;val;});
    printf("%d\n",a);
}