//gcc -Wl,-e,custom_entry entry.c -o program
#include <stdio.h>
int custom_entry(){
    printf("This is called by custom_entry\n");
}