#include "wp_annotation.h"
#include <assert.h>
int hello(int a,int b, int c);
int main(){
int a=10;
a=hello(100,100000,100);
    return a;
}

int f(int a, int b){
return a/b+a%b;
}

int hello(int a,int b, int c){

    _wp_begin();
    for (int i=a;i<b;++i)
    {
       if (i>((a+b)/2)){
         c += a*f(i, i-1);
       }else
       {c += f(i, i+1); }
    }
    _wp_end("119");
return c;
}
