//Libraries
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

//Declared to take space with no reason
const char* ALPH_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* ALPH_LOWER = "abcdefghijklmnopqrstuvwxyz";
const char* DIGITS = "0123456789";
const char* TRACK_MOUSE_EN = "\033[?1003h";
const char* TRACK_MOUSE_DS = "\033[?1000l\033[?1003l";

//Renderer Constants
typedef struct{
    char *text;
    short code;
}rndrConst;
const rndrConst AS_ARROW_UP = { .text="\xE2\x86\x91", .code=255 }; // ↑
const rndrConst AS_ARROW_DN = { .text="\xE2\x86\x93", .code=254 }; // ↓
const rndrConst AS_ARROW_LT = { .text="\xE2\x86\x90", .code=253 }; // ←
const rndrConst AS_ARROW_RT = { .text="\xE2\x86\x92", .code=252 }; // →
const rndrConst AS_SHADE_MIN = { .text="\xE2\x96\x91", .code=251 }; // ░
const rndrConst AS_SHADE_MID = { .text="\xE2\x96\x92", .code=250 }; // ▒
const rndrConst AS_SHADE_FUL = { .text="\xE2\x96\x93", .code=249 }; // ▓
const rndrConst AS_BLOCK_FUL = { .text="\xE2\x96\x88", .code=248 }; // █
const rndrConst AS_BLOCK_UP = { .text="\xE2\x96\x80", .code=247 }; // ▀
const rndrConst AS_BLOCK_DN = { .text="\xE2\x96\x84", .code=246 }; // ▄
const rndrConst AS_BLOCK_RT = { .text="\xE2\x96\x8C", .code=245 }; // ▌
const rndrConst AS_BLOCK_LT = { .text="\xE2\x96\x90", .code=244 }; // ▐
const rndrConst AS_BOX_VL = { .text="\xE2\x94\x82", .code=243 }; // │
const rndrConst AS_BOX_HL = { .text="\xE2\x94\x80", .code=242 }; // ─
const rndrConst AS_BOX_UP_RT = { .text="\xE2\x94\x90", .code=241 }; // ┐
const rndrConst AS_BOX_DN_RT = { .text="\xE2\x94\x98", .code=240 }; // ┘
const rndrConst AS_BOX_UP_LT = { .text="\xE2\x94\x8C", .code=239 }; // ┌
const rndrConst AS_BOX_DN_LT = { .text="\xE2\x94\x94", .code=238 }; // └
const rndrConst AS_BOX_VL_RT = { .text="\xE2\x94\x9C", .code=237 }; // ├
const rndrConst AS_BOX_VL_LT = { .text="\xE2\x94\xA4", .code=236 }; // ┤
const rndrConst AS_BOX_UP_HL = { .text="\xE2\x94\xAC", .code=235 }; // ┬
const rndrConst AS_BOX_DN_HL = { .text="\xE2\x94\xB4", .code=234 }; // ┴
const rndrConst AS_BOX_VL_HL = { .text="\xE2\x94\xBC", .code=233 }; // ┼
const rndrConst AS_BOX_VL_DB = { .text="\xE2\x95\x91", .code=232 }; // ║
const rndrConst AS_BOX_HL_DB = { .text="\xE2\x95\x90", .code=231 }; // ═
const rndrConst AS_BOX_UP_RT_DB = { .text="\xE2\x95\x97", .code=230 }; // ╗
const rndrConst AS_BOX_DN_RT_DB = { .text="\xE2\x95\x9D", .code=229 }; // ╝
const rndrConst AS_BOX_UP_LT_DB = { .text="\xE2\x95\x94", .code=228 }; // ╔
const rndrConst AS_BOX_DN_LT_DB = { .text="\xE2\x95\x9A", .code=227 }; // ╚
const rndrConst AS_BOX_VL_RT_DB = { .text="\xE2\x95\xA0", .code=226 }; // ╠
const rndrConst AS_BOX_VL_LT_DB = { .text="\xE2\x95\xA3", .code=225 }; // ╣
const rndrConst AS_BOX_UP_HL_DB = { .text="\xE2\x95\xA6", .code=224 }; // ╦
const rndrConst AS_BOX_DN_HL_DB = { .text="\xE2\x95\xA9", .code=223 }; // ╩
const rndrConst AS_BOX_VL_HL_DB = { .text="\xE2\x94\xBC", .code=222 }; // ╬
const rndrConst AS_CONGRUENCE = { .text="\xE2\x89\xA1", .code=221 }; // ≡
const rndrConst AS_UNDERSCORE = { .text="\xE2\x80\x96", .code=220 }; // ‗
const rndrConst rndrConsts[] = {
    AS_ARROW_UP, AS_ARROW_DN, AS_ARROW_LT, AS_ARROW_RT, AS_SHADE_MIN, AS_SHADE_MID, AS_SHADE_FUL, AS_BLOCK_FUL, AS_BLOCK_UP, AS_BLOCK_DN, AS_BLOCK_RT, AS_BLOCK_LT, AS_BOX_VL, AS_BOX_HL, AS_BOX_UP_RT, AS_BOX_DN_RT, AS_BOX_UP_LT, AS_BOX_DN_LT, AS_BOX_VL_RT, AS_BOX_VL_LT, AS_BOX_UP_HL, AS_BOX_DN_HL, AS_BOX_VL_HL, AS_BOX_VL_DB, AS_BOX_HL_DB, AS_BOX_UP_RT_DB, AS_BOX_DN_RT_DB, AS_BOX_UP_LT_DB, AS_BOX_DN_LT_DB, AS_BOX_VL_RT_DB, AS_BOX_VL_LT_DB, AS_BOX_UP_HL_DB, AS_BOX_DN_HL_DB, AS_BOX_VL_HL_DB, AS_CONGRUENCE, AS_UNDERSCORE
};
const char* AS_ANGLE_LT = "\xC2\xAB"; // «
const char* AS_ANGLE_RT = "\xC2\xBB"; // »
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
const char* AS_NOT = "\xC2\xAC"; // ¬
const char* AS_REVERSED_NOT = "\xE2\x8C\x90"; // ⌐
const char* AS_ONE_HALF = "\xC2\xBD"; // ½
const char* AS_ONE_QUARTER = "\xC2\xBC"; // ¼
const char* AS_THREE_QUARTERS = "\xC2\xBE"; // ¾
const char* AS_INVERTED_EXCLAMATION = "\xC2\xA1"; // ¡
const char* AS_MACRON = "\xC2\xAF"; // ¯
const char* AS_ACUTE = "\xC2\xB4"; // ´
const char* AS_PLUS_MINUS = "\xC2\xB1"; // ±
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
const char AS_NEUTRAL = '\x01';
enum {
    CV_SHADE_MIN = 176, // ░
    CV_SHADE_MID, // ▒
    CV_SHADE_FUL, // ▓
    CV_BOX_VL, // │
    CV_BOX_VL_LT, // ┤
    CV_BOX_VL_LT_DB = 185, // ╣
    CV_BOX_VL_DB, // ║
    CV_BOX_UP_RT_DB, // ╗
    CV_BOX_DN_RT_DB, // ╝
    CV_BOX_UP_RT = 191, // ┐
    CV_BOX_DN_LT, // └
    CV_BOX_DN_HL, // ┴
    CV_BOX_UP_HL, // ┬
    CV_BOX_VL_RT, // ├
    CV_BOX_HL, // ─
    CV_BOX_VL_HL, // ┼
    CV_BOX_DN_LT_DB = 200, // ╚
    CV_BOX_UP_LT_DB, // ╔
    CV_BOX_DN_HL_DB, // ╩
    CV_BOX_UP_HL_DB, // ╦
    CV_BOX_VL_RT_DB, // ╠
    CV_BOX_HL_DB, // ═
    CV_BOX_VL_HL_DB, // ╬
    CV_BOX_DN_RT = 217, // ┘
    CV_BOX_UP_LT, // ┌
    CV_BLOCK_FUL, // █
    CV_BLOCK_DN, // ▄
    CV_BLOCK_UP = 223, // ▀
    CV_MACRON = 238, // ¯
    CV_ACUTE, // ´
    CV_CONGRUENCE, // ≡
    CV_PLUS_MINUS, // ±
    CV_UNDERSCORE // ‗
};

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
const char *TERM_EVSX[] = { "1003h", "1003l", "1004h", "1004l", "25h", "25l", "12h", "12l", "1049h", "1049l", "1047h", "1047l", "7h", "7l", "1h", "1l", "69h", "69l" };
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
    restoreCursor,
    enableMouseTracking=100,
    disableMouseTracking,
    enableFocusReporting,
    disableFocusReporting,
    showCursor,
    hideCursor,
    cursorBlink,
    cursorNoBlink,
    enableAlternScreen,
    disableAlternScreen,
    switchAlternScreen,
    switchMainScreen,
    enableWrap,
    disableWrap,
    enableKeypadMode,
    disableKeypadMode,
    enableMargins,
    disableMargins
};

void termCtrl(int ev,int n){
    if(ev<100){
        printf("\e[%d%c",n,TERM_EVS[ev]);
    }else{
        printf("\e[?%s",TERM_EVSX[ev]);
    }
}

void cursorPos(int x,int y){
    printf("\e[%d;%dH",y,x);
}

void scrollArea(int top, int bottom){
    printf("\e[%d;%dr",top,bottom);
}

void resetTerminal(){
    printf("\e[!p");
}

void handleSignal(int sig){
    switch (sig){
        case SIGINT:
            printf("Caught Interruption Signal, Exitting...\n");
            exit(1);
            break;
    
        default:
            printf("Caught Signal: %d\n",sig);
            break;
    }
}

enum mouseStates {
    mouseClick,
    mouseMiddle,
    mouseContext,
    mouseDown
};

void newError(char *str,...){
    va_list args;
    va_start(args,str);
    vfprintf(stderr,str,args);
    va_end(args);
    exit(1);
}

//String Properties
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
    if(len==0) len=res.len;
    res.cols=len;
    return res;
}
void debugBuffer(char *buf){
    int len=strlen(buf);
    printf("Buffer of %d bytes: \e[1m",len);
    for(int i=0;i<len;i++){
        printf("%x ",buf[i]);
    }
    printf("\e[0m\n");
}
void printByLen(char *str,int len){
    printf("Printing %d bytes: ",len);
    for(int i=0;i<len;i++){
        printf("%c ",str[i]);
    }
}

//Screen properties 
typedef struct{
    int w;
    int h;
    int len;
    /*int cols;
    int rows;*/
    char *buf;
}screen;
screen newScreen(int w,int h){
    char *buf = (char*)malloc((w/*+1*/)*h+1);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            buf[i*(w/*+1*/)+j]='-';
        }
        //buf[i*(w+1)+w]='\n';
    }
    buf[(w/*+1*/)*h]='\0';
    screen scrn;
    scrn.w=w;
    scrn.h=h;
    scrn.buf=buf;
    scrn.len=w*h;
    /*scrn.cols=w;
    scrn.rows=h;*/
    return scrn;
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
typedef struct{
    unsigned int text:24;
}char24_t;
char24_t char24(char *str){
    char24_t res;
    res.text=0;
    for(int i=0;i<2;i++){
        res.text<<=8;
        res.text|=str[i];
    }
    return res;
}

//Canvas properties
typedef struct{
    int w;
    int h;
    int len;
    char *buf;
}canvas;
canvas newCanvas(int w,int h){
    char *buf = (char*)malloc(w*h+1);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            buf[i*w+j]='-';
        }
        //buf[i*(w+1)+w]='\n';
    }
    buf[w*h]='\0';
    canvas canv;
    canv.w=w;
    canv.h=h;
    canv.buf=buf;
    canv.len=w*h;
    return canv;
}

char *toUTF8(unsigned char *str){
    strInf inf=getStrInf((char*) str);
    char *res=malloc(inf.len*3+1);
    int j=0;
    for(int i=0;i<inf.len;i++){
        if(str[i]<220){
            res[j++]=AS_NEUTRAL;
            res[j++]=AS_NEUTRAL;
            res[j++]=str[i];
        }else{
            res[j++]=rndrConsts[255-str[i]].text[0];
            res[j++]=rndrConsts[255-str[i]].text[1];
            res[j++]=rndrConsts[255-str[i]].text[2];
        }
    }
    return res;
}

//Button properties
typedef struct{
    char *content;
    int id;
    int x;
    int y;
    int cols;
    int rows;
    int xEnd;
    int yEnd;
}button;
button buttons[4];
button newButton(char *cont, int id, int x, int y){
    strInf inf = getStrInf(cont);
    button btn={
        .content=cont,
        .id=id,
        .x=x,
        .y=y,
        .cols=inf.cols,
        .rows=inf.rows,
        .xEnd=x+inf.cols-1,
        .yEnd=y+inf.rows-1
    };
    buttons[id]=btn;
    printf("Button specs: x:%d, y:%d, xend:%d, yend:%d, id:%d, cols:%d, rows:%d\n",x,y,btn.xEnd,btn.yEnd,id,btn.cols,btn.rows);
    return btn;
}
void handleClick(int x, int y){
    for(int i=0;i<sizeof(buttons)/sizeof(button);i++){
        if(x>buttons[i].x-1 && x<buttons[i].xEnd+1 && y>buttons[i].y-1 && y<buttons[i].yEnd+1){
            printf("Button id: #%d clicked at: (%d,%d)\n",buttons[i].id,x,y);
        }
    }
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
    char *res=(char*)malloc(3*len+3*3+1);
    char *p=res;
    *p++=AS_BOX_UP_LT.code;
    int j=0;
    int k=l[j];
    for(int i=0;i<len;i++){
        *p++=i==k?AS_BOX_UP_HL.code:AS_BOX_HL.code;
        if(i==k&&j<n-2) k+=l[++j]+1;
    }
    p+=sprintf(p,"%c\n%c%s",AS_BOX_UP_RT.code,AS_BOX_VL.code,str[0]);
    for(int i=1;i<n;i++){
        p+=sprintf(p,"%c%s",AS_BOX_VL.code,str[i]);
    }
    p+=sprintf(p,"%c\n%c",AS_BOX_VL.code,AS_BOX_DN_LT.code);
    j=0;
    k=l[j];
    for(int i=0;i<len;i++){
        *p++=i==k?AS_BOX_DN_HL.code:AS_BOX_HL.code;
        if(i==k&&j<n-2) k+=l[++j]+1;
    }
    *p++=AS_BOX_DN_RT.code;
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
    char *res=(char*)malloc(3*len+3*3+1);
    char *p=res;
    *p++=AS_BOX_UP_LT_DB.code;
    int j=0;
    int k=l[j];
    for(int i=0;i<len;i++){
        *p++=i==k?AS_BOX_UP_HL_DB.code:AS_BOX_HL_DB.code;
        if(i==k&&j<n-2) k+=l[++j]+1;
    }
    p+=sprintf(p,"%c\n%c%s",AS_BOX_UP_RT_DB.code,AS_BOX_VL_DB.code,str[0]);
    for(int i=1;i<n;i++){
        p+=sprintf(p,"%c%s",AS_BOX_VL_DB.code,str[i]);
    }
    p+=sprintf(p,"%c\n%c",AS_BOX_VL_DB.code,AS_BOX_DN_LT_DB.code);
    j=0;
    k=l[j];
    for(int i=0;i<len;i++){
        *p++=i==k?AS_BOX_DN_HL_DB.code:AS_BOX_HL_DB.code;
        if(i==k&&j<n-2) k+=l[++j]+1;
    }
    *p++=AS_BOX_DN_RT_DB.code;
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

void insertStr(screen scrn,char *str,int x,int y){
    strInf inf=getStrInf(str);
    if(x>scrn.w||y>scrn.h){
        newError("Out of bounds: (%d,%d) in scrnas of %dx%d",x,y,scrn.w,scrn.h);
        return;
    }
    for(int i=0;i<inf.len;i++){
        scrn.buf[((scrn.w/*+1*/)*y)+x+i]=str[i];
    }
}

void display(screen scrn){
    printf("scrnas of %dx%d:\n%s",scrn.w,scrn.h,scrn.buf);
    fflush(stdout);
}

void *task1(void *arg){
    return NULL;
}

void *task2(void *arg){
    return NULL;
}

void init(){
    signal(SIGINT,handleSignal);
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
    char buf[32];
    while (1) {
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
                        handleClick(x,y);
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