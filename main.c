#include <stdio.h>
#include "kurt.h"

int main(int argc, const char **argv) {
    canvas canv = newCanvas(140,20);
    insertStr(canv,"Hello, World!",0,0);
    insertStr(canv,"Hi",1,2);
    display(canv);
    printf("%s\n",color("Hello, World!",4,bold,fg256Colors,5,80));
    printf("%s\n",box("Dildo"));
    printf("%s\n",box("Deldo"));
    printf("%s\n",box_db("Hello, World!"));
    printf("%s\n",box_db("Hello, My World!"));
    printf("\e[38;5;18mHi\e[0m\n",blink);
    return 0;
}