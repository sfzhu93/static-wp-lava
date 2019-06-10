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
        else {
            a += 10;
            if (c/b>3)
                return 0;
        }
    }
    x = a+b+c;
    _wp_end("");
    return x;
}

/*int hello2(int a, int b, int c){
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
        a+=10;
    }

    x = a+b+c;
    _wp_end("");
    return x;
}

int hello3(int a, int b, int c){
    int x;
    _wp_begin();
    if (a<0)
        return 0;
    if (a+b>0)
        return -1;
    if (c<100)
    {
        if (b*c>10)
            b = 1;
        a += 10;
    }

    x = a+b+c;
    _wp_end("");
    return x;
}*/