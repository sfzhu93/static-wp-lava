#include "wp_annotation.h"
#include <assert.h>
static unsigned int lava_val[1000000];

unsigned int lava_get(unsigned int bug_num) {
#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
    if (0x6c617661 - bug_num == lava_val[bug_num] ||
        SWAP_UINT32(0x6c617661 - bug_num) == lava_val[bug_num]) {
        dprintf(1,"Successfully triggered bug %d, crashing now!\n", bug_num);
    }
    else {
        //printf("Not successful for bug %d; val = %08x not %08x or %08x\n", bug_num, lava_val[bug_num], 0x6c617661 + bug_num, 0x6176616c + bug_num);
    }
    return lava_val[bug_num];
}
void lava_set(unsigned int bug_num, unsigned int val) { lava_val[bug_num] = val; }

int hello(int a,int b, int c);
int main(){
int a=10;
a=hello(1,1,1);
    return a;
}

int hello(int a,int b, int c){

    _wp_begin();
    a=5+a+b;
    lava_set(1, c);
    b=a*b;
    c=b*c;
    c=a*b*c+(lava_get(1)==10)*1234567;//if input c == 10, an arithmetic overflow shall happen. Trying to figure it out with WP
    _wp_end("1");
//    assert(("C should be greater than 45", c >= 45));
return c;
}
