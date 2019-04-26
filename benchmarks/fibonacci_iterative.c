//
// Created by suz305 on 4/6/19.
//

#include "wp_annotation.h"

int hello(int n) {
    int a = 1;
    int b = 1;
    int nextTerm = 1;
    _wp_begin();
    for (int i = 2; i <= n; ++i)
    {
        nextTerm = a + b;
        a = b;
        b = nextTerm;
    }
    _wp_end("");
    return b;
}

int main() {
    printf("%d\n", hello(0));
    printf("%d\n", hello(1));
    printf("%d\n", hello(2));
    printf("%d\n", hello(3));
    printf("%d\n", hello(4));
}