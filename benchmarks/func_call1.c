//
// Created by suz305 on 3/21/19.
//

#include "wp_annotation.h"

int max2(int a, int b){
    int ret = 0; // (a>b) and cond(a) or not (a>b) and cond(b)
    if (a>b){
        ret = a;
    }else{
        ret = b;
    }
    return ret;
}

int max(int a, int b, int c){
    int call = max2(a, b); // get (a>b) and cond1(a) or not (a>b) and cond1(b) from max2(a, b)
    //fill cond1(a) and cond1(b) with previous expr:
    //             ==========================================                      ==========================================
    // (a>b) and ( (a>c) and cond(a) or not (a>c) and cond(c) ) or not (a>b) and ( (b>c) and cond(b) or not (b>c) and cond(c) )
    int call1 = max2(call, c); // (call>c) and cond(call) or not (call>c) and cond(c)
    return call1; // cond(call1)
}

int hello(int a, int b, int c, int d){
    _wp_begin();
    int ret = 0;
    if (d>0)
    {
        ret = max(a, b, c);
    }else{
        ret = -max(-a, -b, -c); // ret<12345
    }
    _wp_end("1");
    return ret;
}
