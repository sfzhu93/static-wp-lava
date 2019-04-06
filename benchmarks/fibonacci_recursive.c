/
// Created by suz305 on 3/30/19.
//

#include "wp_annotation.h"

int fib(int n)
{
    if (n==0 || n==1)
        return 1;
    else
        return fib(n-1)+fib(n-2);
}

int hello(int n)
{
    _wp_begin();
    int ret = fib(n);
    _wp_end("");
    return ret;
}
