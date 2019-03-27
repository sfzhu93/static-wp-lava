//
// Created by suz305 on 3/26/19.
//

#include<stdio.h>
#include<math.h>
#include "wp_annotation.h"

double heron(double a, double b, double c){
    float p,area;
    _wp_begin();
    p=1.0/2*(a+b+c);
    if(a+b>c&&b+c>a&&a+c>b){
        area=sqrt(p*(p-a)*(p-b)*(p-c));
    }
    else
        area = 0;
    _wp_end("");
    return area;
}
