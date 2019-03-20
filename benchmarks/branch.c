//
// Created by zsf on 19-3-19.
//

#include "wp_annotation.h"


int f(int a){
    return a+1;
}

int hello(int a, int b, int c){
    _wp_begin();
    a = b+c;
    if(a>2){
        b = (a-1)*(b-1)*(c-1);
    }else{
        b = b*f(a);
    }
    c = a*b*c;
    _wp_end("1");
    return c;
}
