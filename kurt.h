#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <pthread.h>

#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1)
#define COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, N, ...) N

const char* ALPH_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* ALPH_LOWER = "abcdefghijklmnopqrstuvwxyz";
const char* DIGITS = "0123456789";
const char* TRACK_MOUSE_EN = "\033[?1003h\033[?1000h";
const char* TRACK_MOUSE_DS = "\033[?1003l\033[?1000l";
//Renderer Constants
const char* AS_ANGLE_LT = "\xC2\xAB"; // «
const char* AS_ANGLE_RT = "\xC2\xBB"; // »
const char* AS_ARROW_UP = "\xE2\x86\x91"; // ↑
const char* AS_ARROW_DN = "\xE2\x86\x93"; // ↓
const char* AS_ARROW_LT = "\xE2\x86\x90"; // ←
const char* AS_ARROW_RT = "\xE2\x86\x92"; // →
const char* AS_SHADE_MIN = "\xE2\x96\x91"; // ░
const char* AS_SHADE_MID = "\xE2\x96\x92"; // ▒
const char* AS_SHADE_FUL = "\xE2\x96\x93"; // ▓
const char* AS_BLOCK_FUL = "\xE2\x96\x88"; // █
const char* AS_BLOCK_UP = "\xE2\x96\x80"; // ▀
const char* AS_BLOCK_DN = "\xE2\x96\x84"; // ▄
const char* AS_BLOCK_RT = "\xE2\x96\x8C"; // ▌
const char* AS_BLOCK_LT = "\xE2\x96\x90"; // ▐
const char* AS_BOX_VL = "\xE2\x94\x82"; // │
const char* AS_BOX_HL = "\xE2\x94\x80"; // ─
const char* AS_BOX_UP_RT = "\xE2\x94\x90"; // ┐
const char* AS_BOX_DN_RT = "\xE2\x94\x98"; // ┘
const char* AS_BOX_UP_LT = "\xE2\x94\x8C"; // ┌
const char* AS_BOX_DN_LT = "\xE2\x94\x94"; // └
const char* AS_BOX_VL_RT = "\xE2\x94\x9C"; // ├
const char* AS_BOX_VL_LT = "\xE2\x94\xA4"; // ┤
const char* AS_BOX_UP_HL = "\xE2\x94\xAC"; // ┬
const char* AS_BOX_DN_HL = "\xE2\x94\xB4"; // ┴
const char* AS_BOX_VL_HL = "\xE2\x94\xBC"; // ┼
const char* AS_BOX_VL_DB = "\xE2\x95\x91"; // ║
const char* AS_BOX_HL_DB = "\xE2\x95\x90"; // ═
const char* AS_BOX_UP_RT_DB = "\xE2\x95\x97"; // ╗
const char* AS_BOX_DN_RT_DB = "\xE2\x95\x9D"; // ╝
const char* AS_BOX_UP_LT_DB = "\xE2\x95\x94"; // ╔
const char* AS_BOX_DN_LT_DB = "\xE2\x95\x9A"; // ╚
const char* AS_BOX_VL_RT_DB = "\xE2\x95\xA0"; // ╠
const char* AS_BOX_VL_LT_DB = "\xE2\x95\xA3"; // ╣
const char* AS_BOX_UP_HL_DB = "\xE2\x95\xA6"; // ╦
const char* AS_BOX_DN_HL_DB = "\xE2\x95\xA9"; // ╩
const char* AS_BOX_VL_HL_DB = "\xE2\x94\xBC"; // ╬
const char* AS_CURRENCY = "\xC2\xA4"; // ¤
const char* AS_COPYRT = "\xC2\xA9"; // ©
const char* AS_CENT = "\xC2\xA2"; // ¢
const char* AS_POUND = "\xC2\xA3"; // £
const char* AS_YEN = "\xC2\xA5"; // ¥
const char* AS_PESETA = "\xE2\x82\xA7"; // ₧
const char* AS_F_HOOK = "\xC6\x92"; // ƒ
const char* AS_FEMININE_ORDINAL = "\xC2\xAA"; // ª
const char* AS_MASCULINE_ORDINAL = "\xC2\xBA"; // º
const char* AS_INVERTED_QUESTION = "\xC2\xBF"; // ¿
const char* AS_REVERSED_NOT = "\xE2\x8C\x90"; // ⌐
const char* AS_NOT = "\xC2\xAC"; // ¬
const char* AS_ONE_HALF = "\xC2\xBD"; // ½
const char* AS_ONE_QUARTER = "\xC2\xBC"; // ¼
const char* AS_THREE_QUARTERS = "\xC2\xBE"; // ¾
const char* AS_INVERTED_EXCLAMATION = "\xC2\xA1"; // ¡
const char* AS_MACRON = "\xC2\xAF"; // ¯
const char* AS_ACUTE = "\xC2\xB4"; // ´
const char* AS_CONGRUENCE = "\xE2\x89\xA1"; // ≡
const char* AS_PLUS_MINUS = "\xC2\xB1"; // ±
const char* AS_UNDERSCORE = "\xE2\x80\x96"; // ‗
const char* AS_PARAGRAPH = "\xC2\xB6"; // ¶
const char* AS_SECTION = "\xC2\xA7"; // §
const char* AS_DIVISION = "\xC3\xB7"; // ÷
const char* AS_CEDILLA = "\xC2\xB8"; // ¸
const char* AS_DEGREE = "\xC2\xB0"; // °
const char* AS_DIAERESIS = "\xC2\xA8"; // ¨
const char* AS_INTERPUNCT = "\xC2\xB7"; // ·
const char* AS_EXP_ONE = "\xC2\xB9"; // ¹
const char* AS_EXP_TWO = "\xC2\xB2"; // ²
const char* AS_EXP_THR = "\xC2\xB3"; // ³
const char* AS_NBSP = "\xC2\xA0"; //  

enum {
    reset,
    bold,
    dim,
    italian,
    underline,
    blink,
    invert,
    hidden,
    strikeThrough,
    resetBold=21,
    resetDim,
    resetUnderline,
    resetBlink,
    resetInvert,
    resetHidden,
    resetStrikeThrough,
    fgBlack=30,
    fgRed,
    fgGreen,
    fgYellow,
    fgBlue,
    fgMagenta,
    fgCyan,
    fgLightGray,
    fg256Colors,
    fgDefault,
    bgBlack,
    bgRed,
    bgGreen,
    bgYellow,
    bgBlue,
    bgMagenta,
    bgCyan,
    bgLightGray,
    bg256Colors,
    bgDefault,
    fgDarkGray=90,
    fgLightRed,
    fgLightGreen,
    fgLightYellow,
    fgLightBlue,
    fgLightMagenta,
    fgLightCyan,
    fgWhite,
    bgDarkGray=100,
    bgLightRed,
    bgLightGreen,
    bgLightYellow,
    bgLightBlue,
    bgLightMagenta,
    bgLightCyan,
    bgWhite
};

const char *TERM_EVS="ABCDEFGJKSTX@PLMsu";
enum {
    cursorUp,
    cursorDown,
    cursorRight,
    cursorLeft,
    cursorNextLine,
    cursorPrevLine,
    cursorCol,
    eraseDisplay,
    eraseLine,
    scrollUp,
    scrollDown,
    eraseFromPos,
    insertBlank,
    deleteChar,
    insertLine,
    deleteLine,
    saveCursor,
    restoreCursor
};


enum mouseStates {
    mouseClick,
    mouseMiddle,
    mouseContext,
    mouseDown
};

void printByLen(char *str,int len){
    printf("Printing %d bytes: ",len);
    for(int i=0;i<len;i++){
        printf("%c ",str[i]);
    }

}

void newError(char *str,...){
    va_list args;
    va_start(args,str);
    vfprintf(stderr,str,args);
    va_end(args);
    exit(1);
}

void debugBuffer(char *buf){
    int len=strlen(buf);
    printf("Buffer of %d bytes: \e[1m",len);
    for(int i=0;i<len;i++){
        printf("%x ",buf[i]);
    }
    printf("\e[0m\n");
}

typedef struct{
    int len;
    int cols;
    int rows;
}strInf;
strInf getStrInf(char *str){
    strInf res;
    res.len=strlen(str);
    res.cols=0;
    res.rows=1;
    int len=0;
    int lin=0;
    for(int i=0;i<res.len;i++){
        if(str[i]=='\n'||str[i]=='\0'){
            res.rows++;
            if(i-lin>len){
                len=i-lin;
                lin=i;
            }
        }
    }
    res.cols=len;
    return res;
}

typedef struct{
    int w;
    int h;
    int len;
    int cols;
    int rows;
    char *buf;
}canvas;
canvas newCanvas(int w,int h){
    char *buf = (char*)malloc((w+1)*h+1);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            buf[i*(w+1)+j]='-';
        }
        buf[i*(w+1)+w]='\n';
    }
    buf[(w+1)*h]='\0';
    canvas c;
    c.w=w;
    c.h=h;
    c.buf=buf;
    c.len=w*h;
    /*c.cols=w;
    c.rows=h;*/
    return c;
}

strInf screenDim(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    strInf res;
    res.cols=w.ws_col;
    res.rows=w.ws_row;
    res.len=res.cols*res.rows;
    return res;
}

void enableRawMode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag |= (ECHO | ICANON); // Re-enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char *box(int n, ...){
    va_list args;
    va_start(args, n);
    char *str[n];
    for (int i = 0; i < n; i++){
        str[i] = va_arg(args, char *);
    }
    va_end(args);
    int l[n];
    int len=strlen(str[0]);
    l[0]=len;
    for(int i=1;i<n;i++){
        l[i]=strlen(str[i]);
        len+=l[i]+1;
    }
    char *res=(char*)malloc(3*3*len+3*3*3+1);
    char *p=res;
    p+=sprintf(p,"%s",AS_BOX_UP_LT);
    int j=0;
    int k=l[j];
    for(int i=0;i<len;i++){
        p+=sprintf(p,"%s",i==k?AS_BOX_UP_HL:AS_BOX_HL);
        if(i==k&&j<n-2) k+=l[++j]+1;
    }
    p+=sprintf(p,"%s\n%s%s",AS_BOX_UP_RT,AS_BOX_VL,str[0]);
    for(int i=1;i<n;i++){
        p+=sprintf(p,"%s%s",AS_BOX_VL,str[i]);
    }
    p+=sprintf(p,"%s\n%s",AS_BOX_VL,AS_BOX_DN_LT);
    j=0;
    k=l[j];
    for(int i=0;i<len;i++){
        p+=sprintf(p,"%s",i==k?AS_BOX_DN_HL:AS_BOX_HL);
        if(i==k&&j<n-2) k+=l[++j]+1;
    }
    p+=sprintf(p,"%s",AS_BOX_DN_RT);
    return res;
}

char *box_db(int n, ...){
    va_list args;
    va_start(args, n);
    char *str[n];
    for (int i = 0; i < n; i++){
        str[i] = va_arg(args, char *);
    }
    va_end(args);
    int l[n];
    int len=strlen(str[0]);
    l[0]=len;
    for(int i=1;i<n;i++){
        l[i]=strlen(str[i]);
        len+=l[i]+1;
    }
    char *res=(char*)malloc(3*3*len+3*3*3+1);
    char *p=res;
    p+=sprintf(p,"%s",AS_BOX_UP_LT_DB);
    int j=0;
    int k=l[j];
    for(int i=0;i<len;i++){
        p+=sprintf(p,"%s",i==k?AS_BOX_UP_HL_DB:AS_BOX_HL_DB);
        if(i==k&&j<n-2) k+=l[++j]+1;
    }
    p+=sprintf(p,"%s\n%s%s",AS_BOX_UP_RT_DB,AS_BOX_VL_DB,str[0]);
    for(int i=1;i<n;i++){
        p+=sprintf(p,"%s%s",AS_BOX_VL_DB,str[i]);
    }
    p+=sprintf(p,"%s\n%s",AS_BOX_VL_DB,AS_BOX_DN_LT_DB);
    j=0;
    k=l[j];
    for(int i=0;i<len;i++){
        p+=sprintf(p,"%s",i==k?AS_BOX_DN_HL_DB:AS_BOX_HL_DB);
        if(i==k&&j<n-2) k+=l[++j]+1;
    }
    p+=sprintf(p,"%s",AS_BOX_DN_RT_DB);
    return res;
}

char *rgb(int r,int g,int b){
    char *res=malloc(21);
    sprintf(res,"\e[38;2;%d;%d;%dm",r,g,b);
    return res;
}

char *color(char *str, int n, ...){
    va_list args;
    va_start(args, n);
    int clr[n];
    for (int i = 0; i < n; i++){
        clr[i] = va_arg(args, int);
    }
    va_end(args);

    int len = strlen(str);
    char *res = (char *)malloc(len + 10 * n + 5);
    char *p = res;

    p += sprintf(p, "\e[%d", clr[0]);
    for (int i = 1; i < n; i++){
        p += sprintf(p, ";%d", clr[i]);
    }
    p += sprintf(p, "m%s\e[0m", str);
    return res;
}

char* utf8(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4){
    static char buf[5];
    buf[0] = b1;
    buf[1] = b2;
    buf[2] = b3;
    buf[3] = b4;
    buf[4] = '\0';
    return buf;
}

void insertStr(canvas canv,char *str,int x,int y){
    strInf inf=getStrInf(str);
    if(x>canv.w||y>canv.h){
        newError("Out of bounds: (%d,%d) in canvas of %dx%d",x,y,canv.w,canv.h);
        return;
    }
    for(int i=0;i<inf.len;i++){
        canv.buf[((canv.w+1)*y)+x+i]=str[i];
    }
}

void termCtrl(int ev,int n){
    printf("\e[%d%s",n,TERM_EVS[ev]);
}

void cursorPos(int x,int y){
    printf("\e[%d;%dH",y,x);
}

void display(canvas canv){
    printf("Canvas of %dx%d:\n%s",canv.w,canv.h,canv.buf);
}

void *task1(void *arg){
    return NULL;
}

void *task2(void *arg){
    return NULL;
}

void init(){
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, task1, NULL);
    pthread_create(&thread2, NULL, task2, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    enableRawMode();
    atexit(disableRawMode);
    printf(TRACK_MOUSE_EN);
    
    int isDrag=0;
    int lastPos[2];

    printf("Scroll the mouse or press 'q' to quit.\n");
    while (1) {
        char buf[32];
        int n = read(STDIN_FILENO, buf, sizeof(buf));

        if (n > 0) {debugBuffer(buf);
            if (buf[0] == '\033' && buf[1] == '[' && buf[2] == 'M') {
                // Parse mouse event
                int button = buf[3] - 32;
                int x = buf[4] - 32;
                int y = buf[5] - 32;

                if (button == 64) { // Scroll up (button code: 64)
                    printf("Scroll up detected at (%d, %d)\n", x, y);
                } else if (button == 65) { // Scroll down (button code: 65)
                    printf("Scroll down detected at (%d, %d)\n", x, y);
                } else {
                    printf("Mouse event: Button=%d, X=%d, Y=%d\n", button, x, y);
                    if(isDrag){
                        printf("Dragged by: (%d,%d)\n",x-lastPos[0],y-lastPos[1]);
                    }else{
                        lastPos[0]=x;
                        lastPos[1]=y;
                    }
                    isDrag=!isDrag;
                }
            }
            if (buf[0] == 'q') {
                break;
            }
        }
    }

    printf(TRACK_MOUSE_DS);
}