#include <stdio.h>
#include "kurt.h"

int main(int argc, const char **argv) {
    canvas canv = newCanvas(80,20);
    insert(canv,"Hello, World!",1,1);
    insert(canv,"Hi",1,2);
    display(canv);
    printf("Hello, World!\n");
    box("5");
    printf("\e[38;5;18mHi\e[0m\n",blink);
    return 0;
}