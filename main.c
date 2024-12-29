#include <stdio.h>
#include <unistd.h>
#include "kurt.h"

void funn(){
    char *p=box_db(5,"Abcd","Bar","Dooodoo","Daddy","FFFF");
    for(int i=0;i<8;i++){
        system("clear");
        printf("%s\n",color(p,4,bold,italian,fgDarkGray+i,blink));
        usleep(5e5);
    }
}

void func(int n, ...){
    va_list args;
    va_start(args, n);
    char *str[n];
    for (int i = 0; i < n; i++){
        str[i] = va_arg(args, char *);
    }
    va_end(args);
    for (int i = 0; i < n; i++){
        printf("%s\n",str[i]);
    }
}

void mainMain(){
    printf("Algorithmic Project by %s\n\n",color("Sidali Kurt",4,bold,blink,fgBlue,italian));
    printf("\tChoose Project:\n\t\t%s %s\n\t\t%s %s\n",AS_ARROW_RT,color("BTree",4,underline,bold,fgYellow,blink),AS_ARROW_RT,color("PoW",4,underline,bold,fgYellow,blink));
}

int main(int argc, const char **argv) {
    strInf dim = screenDim();
    canvas canv = newCanvas(dim.cols,dim.rows);//(150,20);
    insertStr(canv,"Hello, World!",0,0);
    insertStr(canv,"Hi",1,2);
    button btn=newButton("BTN",5,3,3);
    insertStr(canv,"BTN",3,3);
    display(canv);
    printf("%s\n%sSome text\e[0m\n",color("Hello, World!",4,bold,fg256Colors,5,80),rgb(255, 0, 208));
    printf("%s\n",box(5,"Abcd","Bar","Dooodoo","Daddy","FFFF"));
    printf("%s\n",color(box_db(5,"Abcd","Bar","Dooodoo","Daddy","FFFF"),4,bold,italian,fgLightMagenta,blink));
    printf("%s\n",color(box_db(5,"Abcd","Bar","Dooodoo","Daddy","FFFF"),4,bold,italian,fgGreen,blink));
    printf("%s\n",box(1,"Bar"));
    printf("%s\n",box_db(1,"Hello, World!"));
    printf("%s\n",box_db(1,"Hello, My World!"));
    func(3,"Hello","World","!22");
    //funn();
    init();
    mainMain();
    return 0;
}