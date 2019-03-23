//
// Created by suz305 on 3/23/19.
//
#include "wp_annotation.h"

int hello(int a, int b, int c){
    int x;
    _wp_begin();
    if (a<0)
        return 0;
    if (a+b>0)
        return -1;
    if (c<100)
    {
        if (b*c>10)
            return 0;
    }
    x = a+b+c;
    _wp_end("");
    return x;
}