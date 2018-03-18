#include<stdio.h>
/*
Test gcc optimization levels, below can can see the assembler code.
No opt: 
gcc -S opt.c -O0 -o opto0.s

A little opt: 
gcc -S opt.c -O1 -o opto1.s 
*/
int main(){
    int x = 0;
    x += 1;
    x += 1;
    x += 1;
    printf("%d\n", x);
    return 0;
}