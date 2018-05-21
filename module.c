#include <stdio.h>

int five()  {
    return 5;
}

int fib(int n)  {
    if(n < 3)   {
        return 1;
    } else  {
        return fib(n-1) + fib(n-2);
    }
}

void say_hello()    {
    printf("hello world!\n");fflush(stdout);
}

void print_msg(char *msg)   {
    printf("%s\n", msg);
}
