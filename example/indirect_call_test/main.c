#include<stdio.h>

void (*fun_ptr)(int);

void fun(int a)
{
     printf("Value of a: %d\n", a);
}

int func_3(int a, int b)
{
    (*fun_ptr)(a+b);
}

int main ()
{
    fun_ptr = fun;
    func_3(4, 5);
}
