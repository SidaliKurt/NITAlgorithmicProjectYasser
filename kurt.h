#include <stdio.h>
#include <stdlib.h>






char* utf8(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4){
    static char buf[5];
    buf[0] = b1;
    buf[1] = b2;
    buf[2] = b3;
    buf[3] = b4;
    buf[4] = '\0';
    return buf;
}