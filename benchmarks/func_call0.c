//
// Created by suz305 on 3/21/19.
//

#include "wp_annotation.h"

int max2(int a, int b){
    int ret = 0;  //(a>b) and cond(a) or not (a>b) and cond(b)
    if (a>b){
        ret = a;
    }else{
        ret = b;
    }
    return ret;   //cond(ret)
}

int hello(int a, int b, int c){
    _wp_begin();
    int ret = 0;
    if (c>0)
    {
        ret = max2(a, b); //(a>b) and (a<12345) or not (a>b) and (b<12345)
    }else{
        ret = -max2(-a, -b); // we replace cond(a) and cond(b) in line 8 with WP and args: (-a>-b) and (-a<12345) or not (-a>-b) and (-b<12345)
    }
    //WP: ret<12345
    _wp_end("1");
    return ret;
}
