//
// Created by suz305 on 4/1/19.
//

#include "wp_annotation.h"
#include <stdlib.h>

struct node{
    int value;
    struct node *next;
};

struct node* hello(int a, int b, int c) {
    struct node *ret;
    ret = (struct node *)malloc(sizeof(struct node));
    ret->value = a;
    ret->next = (struct node *)malloc(sizeof(struct node));
    ret->next->value = b;
    ret->next->next = (struct node *)malloc(sizeof(struct node));
    ret->next->next->value = c;
    ret->next->next->next = NULL;
    return ret;
}

int sum(struct node * head){
    int ret = 0;
    int a = head->value;
    int b = head->next->value;
    int c = head->next->next->value;
    ret = a*10000+b*100+c;
    return ret;
}

int doit( int a, int b, int c) {
    _wp_begin();
    struct node *head = hello(a, b, c);
    int s = sum(head);
    _wp_end("");
    return s;
}