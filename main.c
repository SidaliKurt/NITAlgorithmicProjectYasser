#include <stdio.h>
#include <unistd.h>
#include "kurt.h"
#include "btree.h"
#include "pow.h"


const char *btreeBanner="\e[1;5;34mBBBBBBBBBBBBBBBBB   TTTTTTTTTTTTTTTTTTTTTTT                                                            \n"
    "B::::::::::::::::B  T:::::::::::::::::::::T                                                            \n"
    "B::::::BBBBBB:::::B T:::::::::::::::::::::T                                                            \n"
    "BB:::::B     B:::::BT:::::TT:::::::TT:::::T                                                            \n"
    "  B::::B     B:::::BTTTTTT  T:::::T  TTTTTTrrrrr   rrrrrrrrr       eeeeeeeeeeee        eeeeeeeeeeee    \n"
    "  B::::B     B:::::B        T:::::T        r::::rrr:::::::::r    ee::::::::::::ee    ee::::::::::::ee  \n"
    "  B::::BBBBBB:::::B         T:::::T        r:::::::::::::::::r  e::::::eeeee:::::ee e::::::eeeee:::::ee\n"
    "  B:::::::::::::BB          T:::::T        rr::::::rrrrr::::::re::::::e     e:::::ee::::::e     e:::::e\n"
    "  B::::BBBBBB:::::B         T:::::T         r:::::r     r:::::re:::::::eeeee::::::ee:::::::eeeee::::::e\n"
    "  B::::B     B:::::B        T:::::T         r:::::r     rrrrrrre:::::::::::::::::e e:::::::::::::::::e \n"
    "  B::::B     B:::::B        T:::::T         r:::::r            e::::::eeeeeeeeeee  e::::::eeeeeeeeeee  \n"
    "  B::::B     B:::::B        T:::::T         r:::::r            e:::::::e           e:::::::e           \n"
    "BB:::::BBBBBB::::::B      TT:::::::TT       r:::::r            e::::::::e          e::::::::e          \n"
    "B:::::::::::::::::B       T:::::::::T       r:::::r             e::::::::eeeeeeee   e::::::::eeeeeeee  \n"
    "B::::::::::::::::B        T:::::::::T       r:::::r              ee:::::::::::::e    ee:::::::::::::e  \n"
    "BBBBBBBBBBBBBBBBB         TTTTTTTTTTT       rrrrrrr                eeeeeeeeeeeeee      eeeeeeeeeeeeee  \e[0m\n";

const char *powBanner="\e[1;5;33mPPPPPPPPPPPPPPPPP                WWWWWWWW                           WWWWWWWW\n"
    "P::::::::::::::::P               W::::::W                           W::::::W\n"
    "P::::::PPPPPP:::::P              W::::::W                           W::::::W\n"
    "PP:::::P     P:::::P             W::::::W                           W::::::W\n"
    "  P::::P     P:::::P  ooooooooooo W:::::W           WWWWW           W:::::W \n"
    "  P::::P     P:::::Poo:::::::::::ooW:::::W         W:::::W         W:::::W  \n"
    "  P::::PPPPPP:::::Po:::::::::::::::oW:::::W       W:::::::W       W:::::W   \n"
    "  P:::::::::::::PP o:::::ooooo:::::o W:::::W     W:::::::::W     W:::::W    \n"
    "  P::::PPPPPPPPP   o::::o     o::::o  W:::::W   W:::::W:::::W   W:::::W     \n"
    "  P::::P           o::::o     o::::o   W:::::W W:::::W W:::::W W:::::W      \n"
    "  P::::P           o::::o     o::::o    W:::::W:::::W   W:::::W:::::W       \n"
    "  P::::P           o::::o     o::::o     W:::::::::W     W:::::::::W        \n"
    "PP::::::PP         o:::::ooooo:::::o      W:::::::W       W:::::::W         \n"
    "P::::::::P         o:::::::::::::::o       W:::::W         W:::::W          \n"
    "P::::::::P          oo:::::::::::oo         W:::W           W:::W           \n"
    "PPPPPPPPPP            ooooooooooo            WWW             WWW            \e[0m\n";

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

canvas btreeCanv;
void btreeMain(){
    termCtrl(eraseDisplay,2);
    btreeCanv=newCanvas(primScreen.w,primScreen.h);
    BTreeNode* root = NULL;
    clearCanvas(btreeCanv,0x20);
    
    insertStrc(btreeCanv,box(2,"Input:","     "),1,0);
    insertStrc(btreeCanv,box_db(1,"Insert"),16,0);
    insertStrc(btreeCanv,box_db(1,"Remove"),25,0);
    insertStrc(btreeCanv,box_db(1,"Search"),34,0);
    insertStrc(btreeCanv,box_db(1,"Traverse"),43,0);
    fill(btreeCanv,CV_UNDERSCORE,0,3,primScreen.w-1,3);
    display(btreeCanv,primScreen);
    
    
}

void showBlock();
void pow_wait_next(){
    void onClick(int ev, int x, int y){printf("\n\e[1A");
        if(ev==mouseClick&&x>30&&x<40&&y>3&&y<8){
            ALLOW_MOUSE=0;
            showBlock();
        }
    }
    addEventListener(onClick);
}

canvas powCanv;
void showBlock(){
    block *blk=lastBlock=getLastBlock(firstBlock);
    termCtrl(eraseDisplay,2);
    enableRawMode();
    clearCanvas(powCanv,0x20);
    char b[3];
    sprintf(b,"%c%c",CV_BLOCK_FUL,CV_BLOCK_FUL);
    insertStrc(powCanv,b,0,1);
    insertStrc(powCanv,box_db(1," #0 "),2,0);
    int d=2;
    char p[5];
    for(int i=1;i<blklen(firstBlock);i++){
        d+=6;
        fill(powCanv,CV_CONGRUENCE,d,1,d+4,1);
        d+=5;
        sprintf(p," #%d ",i);
        insertStrc(powCanv,box_db(1,p),d,0);
    }
    fill(powCanv,CV_UNDERSCORE,0,3,primScreen.w-1,3);
    insertStrc(powCanv,box_db(2,"Block No:"," # "),1,4);
    insertStrc(powCanv,box_db(1," MINE "),30,4);
    insertStrc(powCanv,box_db(2,"Nounce:","            "),53,4);
    insertStrc(powCanv,"Previous Hash: ",1,7);
    insertStrc(powCanv,"Timestamp: ",1,8);
    insertStrc(powCanv,box(2,"Transactions:","                                                          "),1,9);
    insertStrc(powCanv,box_db(2,"Hash:","                                                                  "),1,12);
    display(powCanv,primScreen);
    cursorPos(14,6);
    printf("%d\n",blk->id);
    cursorPos(17,8);
    printf("%s\n",blk->prevHash);
    cursorPos(13,9);
    printf("%s\n", ctime(&(blk->timestamp)));
    void onClick(int ev, int x, int y){printf("\n\e[1A");
        if(ev==mouseClick){
            if(x<74&&y>9&&y<13){
                disableRawMode();
                cursorPos(18,11);
                //printf("Transaction\n");
                
                //cursorPos(18,11);//re op
                char trans[66];
                scanf("%s",&trans);
                blk->transactions=trans;
                enableRawMode();
            }
            if(x>30&&x<40&&y>3&&y<8){
                disableRawMode();
                printf(TRACK_MOUSE_DS);
                cursorPos(32,6);
                printf("\e[%d;%dm MINE\e[0m\n",blink,fgLightRed);
                void onChange(int nonce, uint8_t *digest){
                    cursorPos(64,6);
                    printf("%d\n",nonce);
                    cursorPos(10,14);
                    for (int i = 0; i < 32; i++){
                        printf("%02x", digest[i]);
                    } 
                }
                mineBlock(blk,onChange);
                cursorPos(64,6);
                printf("%d\n",blk->nonce);
                cursorPos(10,14);
                printf("%s\n",blk->hash);
                cursorPos(32,6);
                printf("\e[1;%dm NEXT\e[0m\n",fgLightGreen);
                ALLOW_MOUSE=0;
                pow_wait_next();
            }
            
        }
    }
    addEventListener(onClick);
}

void powMain(){
    termCtrl(eraseDisplay,2);
    disableRawMode();
    powCanv = newCanvas(primScreen.w,primScreen.h);
    clearCanvas(powCanv,0x20);
    int l=insertStrc(powCanv,"Set Difficulty: ",0,1);
    insertStrc(powCanv,inputBox(5,1),l+1,0);
    display(powCanv,primScreen);
    cursorPos(l+3,2);
    scanf("%u",&difficulty);
    if(difficulty>64){
        printf("\nToo Huge number!!");
        sleep(5);
        powMain();
    }
    firstBlock=newBlock(NULL);
    showBlock();
    
}

void mainMenu(){
    termCtrl(eraseDisplay,2);
    printf("Algorithmic Project by %s\n\n",color("Sidali Kurt",4,bold,blink,fgBlue,italian));
    printf("\tChoose Project (use scroller or arrows then enter):\n\t\t%s %s\n\t\t%s %s\n",AS_ARROW_RT.text,color("BTree",4,underline,bold,fgYellow,blink),AS_ARROW_RT.text,color("PoW",4,underline,bold,fgYellow,blink));
    int choice=-1;
    void chooseProject(int evt,int x, int y){
        if(evt==mouseScrollUp||evt==kbdArrowUp){
            choice=0;
            termCtrl(eraseDisplay,2);
            printf("%s\n",btreeBanner);
        }else if(evt==mouseScrollDown||evt==kbdArrowDown){
            choice=1;
            termCtrl(eraseDisplay,2);
            printf("%s\n",powBanner);
        }else if(evt==kbdEnter&&choice>-1){
            ALLOW_MOUSE=0;
            termCtrl(eraseDisplay,2);
            printf("Choice: %d\n",choice);
            if(choice==0){
                btreeMain();
            }else{
                powMain();
            }
            //choice==1?:;
        }
    }
    addEventListener(chooseProject);

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
    termCtrl(eraseDisplay,2);
    strInf dim = screenDim();
    screen scrn = newScreen(dim.cols,dim.rows);//(152,12);
    primScreen=scrn;
    clrset=(colorset*)malloc(scrn.len*sizeof(colorset));
    /*
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
    */

    //funn();
    init();
    mainMenu();
    /*printf("%s\n",color("ButtonClickable",2,bold,invert));
    colorset clrs=color_m("ButtonClickable",2,2,2,bold,bgCyan);
    color_m("ButtonVVVV",5,5,2,bold,bgGreen);
    insertStr(scrn,"Button",primScreen.w-6,primScreen.h-1);
    color_m("Button",primScreen.w-6,primScreen.h-1,2,bold,bgLightRed);
    print(scrn);
    printf("\n%s\n%s\n",toUTF8(box_db(2,"Elm","Frac")),toUTF8(box_db(3,"1","2","3")));
    //sha_256();
    canvas canv=newCanvas(dim.cols,dim.rows);
    for(int i=0;i<50;i++){
        clearCanvas(canv,0x20);
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
    //addEventListener(mouseMove,0,0,showBtn);
    mainMenu();*/
    return 0;
}