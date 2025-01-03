#include <stdio.h>
#include <unistd.h>
#include "kurt.h"
#include "btree.h"
#include "sha256.h"

void about(){
    printf("This is a project by Sidali Kurt\n");
}

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

void mainMenu(){
    termCtrl(eraseDisplay,2);
    printf("Algorithmic Project by %s\n\n",color("Sidali Kurt",4,bold,blink,fgBlue,italian));
    printf("\tChoose Project:\n\t\t%s %s\n\t\t%s %s\n",AS_ARROW_RT.text,color("BTree",4,underline,bold,fgYellow,blink),AS_ARROW_RT.text,color("PoW",4,underline,bold,fgYellow,blink));
    int f;
    scanf("%d",&f);

}

void sha_256(){
    const char *message = "Hello, SHA-256!";
    uint8_t digest[32];

    sha256((const uint8_t *)message, strlen(message), digest);

    printf("SHA-256 Digest: ");
    for (int i = 0; i < 32; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");
}

int main(int argc, const char **argv) {
    termCtrl(eraseDisplay,0);
    strInf dim = screenDim();
    screen scrn = newScreen(dim.cols,dim.rows);//(152,12);
    primScreen=scrn;
    clrset=(colorset*)malloc(scrn.len*sizeof(colorset));
    
    insertStr(scrn,"Hello, World!",0,0);
    insertStr(scrn,"Hi",1,2);
    button btn=newButton("BTN",0,3,3);
    insertStr(scrn,"BTN",3,3);
    button btn2=newButton(color("ButtonClickable",2,bold,invert),1,50,5);
    //insertStr(scrn,btn2.content,btn2.x,btn2.y);
    
    printf("%s\n%sSome text\e[0m\n",color("Hello, World!",4,bold,fg256Colors,5,80),rgb(255, 0, 208));
    printf("%s\n",toUTF8(box(5,"Abcd","Bar","Dooodoo","Daddy","FFFF")));
    printf("%s\n",toUTF8(box(5,"Abcd","Bar","Dooodoo","Daddy","FFFF")));
    printf("%s\n",toUTF8(color(box_db(5,"Abcd","Bar","Dooodoo","Daddy","FFFF"),4,bold,italian,fgLightMagenta,blink)));
    printf("%s\n",toUTF8(color(box_db(5,"Abcd","Bar","Dooodoo","Daddy","FFFF"),4,bold,italian,fgGreen,blink)));
    printf("%s\n",toUTF8(box(1,"Bar")));
    printf("%s\n",toUTF8(box_db(1,"Hello, World!")));
    printf("%s\n",toUTF8(box_db(1,"Hello, My World!")));
    func(3,"Hello","World","!22");


    //funn();
    init();
    mainMenu();
    printf("%s\n",color("ButtonClickable",2,bold,invert));
    colorset clrs=color_m("ButtonClickable",2,2,2,bold,bgCyan);
    color_m("ButtonVVVV",5,5,2,bold,bgGreen);
    insertStr(scrn,"Button",primScreen.w-6,primScreen.h-1);
    color_m("Button",primScreen.w-6,primScreen.h-1,2,bold,bgLightRed);
    print(scrn);
    printf("\n%s\n%s\n",toUTF8(box_db(2,"Elm","Frac")),toUTF8(box_db(3,"1","2","3")));
    //sha_256();
    canvas canv=newCanvas(dim.cols,dim.rows);
    for(int i=0;i<50;i++){
        clearCanvas(canv,'\x20');
        termCtrl(eraseDisplay,2);
        button btn = newButton(box_db(2,"Hello","World!"),0,i,7);
        insertStrc(canv,btn.content,i,7);
        drawLine(&canv,0,0,i,6);
        drawLine(&canv,dim.cols-1,0,i,6);
        display(canv,primScreen);
        //usleep(1e5);
    }
    void showBtn(int x, int y){
        clearCanvas(canv,0x20);
        termCtrl(eraseDisplay,2);
        x-=btn.cols/2+2;
        y-=btn.rows/2+1;
        button btn = newButton(box_db(1,"Button"),0,x,y);
        insertStrc(canv,btn.content,x,y);
        drawLine(&canv,dim.cols/2,0,x+btn.cols/2,y-1);
        display(canv,primScreen);
    }
    showBtn(50,5);
    addEventListener(mouseMove,0,0,showBtn);
    mainMenu();
    return 0;
}