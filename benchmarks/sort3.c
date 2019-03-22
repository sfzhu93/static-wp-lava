//
// Created by suz305 on 3/22/19.
//
#include "wp_annotation.h"

__attribute__((noinline))
int sort3(int a, int b, int c)
{
    _wp_begin();
    int t;
    if(a>b)
    {t=a;a=b;b=t;}
    if(a>c)
    {t=a;a=c;c=t;}
    if(b>c)
    {t=b;b=c;c=t;}
    int ret = a*10000+b*100+c;
    _wp_end("");
    return ret;
}

int main()
{
    int a,b,c,t;
    scanf("%d,%d,%d",&a,&b,&c);
    t = sort3(a, b, c);
    printf("%d",t);
    return 0;
}



