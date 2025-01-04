//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
const rndrConst AS_ARROW_UP_LT = { .text="\xE2\x86\x96", .code=219 }; // ↖
const rndrConst AS_ARROW_UP_RT = { .text="\xE2\x86\x97", .code=218 }; // ↗
const rndrConst AS_ARROW_DN_RT = { .text="\xE2\x86\x98", .code=217 }; // ↘
const rndrConst AS_ARROW_DN_LT = { .text="\xE2\x86\x99", .code=216 }; // ↙
const rndrConst rndrConsts[] = {
    AS_ARROW_UP, AS_ARROW_DN, AS_ARROW_LT, AS_ARROW_RT, AS_SHADE_MIN, AS_SHADE_MID, AS_SHADE_FUL, AS_BLOCK_FUL, AS_BLOCK_UP, AS_BLOCK_DN, AS_BLOCK_RT, AS_BLOCK_LT, AS_BOX_VL, AS_BOX_HL, AS_BOX_UP_RT, AS_BOX_DN_RT, AS_BOX_UP_LT, AS_BOX_DN_LT, AS_BOX_VL_RT, AS_BOX_VL_LT, AS_BOX_UP_HL, AS_BOX_DN_HL, AS_BOX_VL_HL, AS_BOX_VL_DB, AS_BOX_HL_DB, AS_BOX_UP_RT_DB, AS_BOX_DN_RT_DB, AS_BOX_UP_LT_DB, AS_BOX_DN_LT_DB, AS_BOX_VL_RT_DB, AS_BOX_VL_LT_DB, AS_BOX_UP_HL_DB, AS_BOX_DN_HL_DB, AS_BOX_VL_HL_DB, AS_CONGRUENCE, AS_UNDERSCORE, AS_ARROW_UP_LT, AS_ARROW_UP_RT, AS_ARROW_DN_RT, AS_ARROW_DN_LT
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
    CV_ARROW_DN_LT = 216, // ↙
    CV_ARROW_DN_RT, // ↘
    CV_ARROW_UP_RT, // ↗
    CV_ARROW_UP_LT, // ↖
    CV_UNDERSCORE, // ‗
    CV_CONGRUENCE, // ≡ 
    CV_BOX_VL_HL_DB, // ╬
    CV_BOX_DN_HL_DB, // ╩
    CV_BOX_UP_HL_DB, // ╦
    CV_BOX_VL_LT_DB, // ╣
    CV_BOX_VL_RT_DB, // ╠
    CV_BOX_DN_LT_DB, // ╚
    CV_BOX_UP_LT_DB, // ╔
    CV_BOX_DN_RT_DB, // ╝
    CV_BOX_UP_RT_DB, // ╗
    CV_BOX_HL_DB, // ═
    CV_BOX_VL_DB, // ║
    CV_BOX_VL_HL, // ┼
    CV_BOX_DN_HL, // ┴
    CV_BOX_UP_HL, // ┬
    CV_BOX_VL_LT, // ┤
    CV_BOX_VL_RT, // ├
    CV_BOX_DN_LT, // └
    CV_BOX_UP_LT, // ┌
    CV_BOX_DN_RT, // ┘
    CV_BOX_UP_RT, // ┐
    CV_BOX_HL, // ─
    CV_BOX_VL, // │
    CV_BLOCK_LT, // ▐
    CV_BLOCK_RT, // ▌
    CV_BLOCK_DN, // ▄
    CV_BLOCK_UP, // ▀
    CV_BLOCK_FUL, // █
    CV_SHADE_FUL, // ▓
    CV_SHADE_MID, // ▒
    CV_SHADE_MIN, // ░
    CV_ARROW_RT, // →
    CV_ARROW_LT, // ←
    CV_ARROW_DN, // ↓
    CV_ARROW_UP, // ↑
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
    mouseUp,
    mouseMove,
    mouseDrag,
    otherChars,
    kbdArrowUp=41,
    kbdArrowDown,
    kbdArrowRight,
    kbdArrowLeft,
    kbdEnter,
    kbdBackspace,
    mouseScrollUp=64,
    mouseScrollDown
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
    char *buf = (char*)malloc((w/*+1*/)*h*3+1);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            buf[i*(w/*+1*/)+j]='-';
        }
        //buf[i*(w+1)+w]='\n';
    }
    buf[(w/*+1*/)*h*3]='\0';
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
screen primScreen;
screen secdScreen;

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
void clearCanvas(canvas canv, char fill){
    for(int i=0;i<canv.h;i++){
        for(int j=0;j<canv.w;j++){
            canv.buf[i*canv.w+j]=fill;
        }
    }
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
void drawLine(canvas *canv, int x1, int y1, int x2, int y2) {
    if (x1 < 0 || x1 >= canv->w || x2 < 0 || x2 >= canv->w || y1 < 0 || y1 >= canv->h || y2 < 0 || y2 >= canv->h) {
        printf("Error: Points out of canvas bounds.\n");
        return;
    }
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int err2;
    char symbol = '-';
    while (1) {
        // Precompute slope-dependent character selection
        if (dx > dy) {
            symbol = sy > 0 && sx < 0? '/' : '\\'; // Gentle slope
        } else if (dy > dx) {
            symbol = '|'; // Steep slope
        }
        // Draw point
        canv->buf[y1 * canv->w + x1] = symbol;
        // Exit condition
        if (x1 == x2 && y1 == y2) break;
        err2 = 2 * err;
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}



void display(canvas canvasFrom,screen screenTo){
    sprintf(screenTo.buf,toUTF8((unsigned char*)canvasFrom.buf));
    printf(screenTo.buf);
    fflush(stdout);
}

void display_m(screen scrn){
    printf(scrn.buf);
    fflush(stdout);
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
    //printf("Button specs: x:%d, y:%d, xend:%d, yend:%d, id:%d, cols:%d, rows:%d\n",x,y,btn.xEnd,btn.yEnd,id,btn.cols,btn.rows);
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

char *inputBox(int cols, int rows){
    char *str=malloc(cols+rows+1);
    for(int i=0;i<cols;i++) str[i]=0x20;
    for(int i=0;i<rows-1;i++) str[i]='\n';
    return box(1,str);
}

void fill(canvas canv,char fil, int x1, int y1, int x2, int y2){
    if(x1<0||x1>=canv.w||x2<0||x2>=canv.w||y1<0||y1>=canv.h||y2<0||y2>=canv.h){
        newError("Out of bounds: (%d,%d) to (%d,%d) in canvas of %dx%d",x1,y1,x2,y2,canv.w,canv.h);
        return;
    }
    for(int i=y1;i<=y2;i++){
        for(int j=x1;j<=x2;j++){
            canv.buf[i*canv.w+j]=fil;
        }
    }
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
    char *res = (char *)malloc(len + 4 * n + 5);
    char *p = res;

    p += sprintf(p, "\e[%d", clr[0]);
    for (int i = 1; i < n; i++){
        p += sprintf(p, ";%d", clr[i]);
    }
    p += sprintf(p, "m%s\e[0m", str);
    return res;
}

typedef struct{
    char *clrs;
    int x;
    int y;
}colorset;
colorset *clrset;
int clrsetLen=0;
colorset color_m(char *str,int x, int y, int n,...){
    va_list args;
    va_start(args, n);
    int clr[n];
    for (int i = 0; i < n; i++){
        clr[i] = va_arg(args, int);
    }
    va_end(args);

    int len = strlen(str);
    colorset res = { .clrs = (char *)malloc(4 * n -1), .x = x, .y = y };
    char *p = res.clrs;
    p += sprintf(p, "%d", clr[0]);
    for (int i = 1; i < n; i++){
        p += sprintf(p, ";%d", clr[i]);
    }
    clrset[clrsetLen++]=res;
    clrset[clrsetLen++]=(colorset){.clrs="0",.x=x+len,.y=y};
    return res;
}
void printColor(int x, int y){
    for(int i=0;i<clrsetLen;i++){
        if(clrset[i].x==x&&clrset[i].y==y){
            printf("\e[%sm",clrset[i].clrs);
        }
    }
}
void print(screen scrn){
    int x,y=0;
    for(int i=0;i<scrn.len;i++){
        x=i%scrn.w;
        y=i/scrn.w;
        printColor(x,y);
        printf("%c",scrn.buf[i]);
    }
    printf("\e[0m");
}

void printColors(){
    for(int i=0;i<sizeof(clrset)/sizeof(colorset)-1;i++){
        cursorPos(clrset[i].x,4);
        printf("\e[%sm\e[%d@",clrset[i].clrs,clrset[i].y,clrset[i].y);
        cursorPos(0,12);
        usleep(1e6);
        
    }
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

int insertStrc(canvas canv,char *str,int x,int y){
    strInf inf=getStrInf(str);
    if(x>canv.w||y>canv.h){
        newError("Out of bounds: (%d,%d) in canvas of %dx%d",x,y,canv.w,canv.h);
        return 0;
    }
    int j=0;
    for(int i=0;i<inf.len;i++){
        if(str[i]=='\n'){
            j+=canv.w-inf.cols;
            continue;
        }
        canv.buf[canv.w*y+x+j+i]=str[i];
    }
    return inf.len;
}

void moveCanvas(canvas canv,screen scrn, int dx, int dy){
    char *buf=toUTF8((unsigned char*)canv.buf);
    for(int j=0;j<scrn.h;j++){
        for(int i=0;i<scrn.w;i++){
            scrn.buf[scrn.w*j+i]=buf[canv.w*(j+dy)+i+dx];
        }
    }
}

void moveStr(canvas canv,int dx, int dy){

}

int ALLOW_MOUSE=1;
void addEventListener(void (*callback)(int,int,int)){
    printf(TRACK_MOUSE_EN);
    int isDrag=0;
    int lastPos[2];
    ALLOW_MOUSE=1;
    //printf("Scroll the mouse or press 'q' to quit.\n");
    char buf[32];
    int button,x,y;
    while (ALLOW_MOUSE) {
        if (read(STDIN_FILENO, buf, sizeof(buf))) {//debugBuffer(buf);
            if (buf[0] == '\033' && buf[1] == '['){
                if(buf[2] == 'M') {
                    button = buf[3] - 32;
                    x = buf[4] - 32;
                    y = buf[5] - 32;
                    if (button == mouseScrollUp) {
                        //printf("Scroll up detected at (%d, %d)\n", x, y);
                        callback(mouseScrollUp,x,y);
                    } else if (button == mouseScrollDown) {
                        //printf("Scroll down detected at (%d, %d)\n", x, y);
                        callback(mouseScrollDown,x,y);
                    } else if (button == mouseClick) {
                        //printf("Left click detected at (%d, %d)\n", x, y);
                        callback(mouseClick,x,y);
                    } else if (button == mouseUp){
                        //printf("Mouse up detected at (%d, %d)\n", x, y);
                        if(isDrag&&(x-lastPos[0]||y-lastPos[1])){
                            //printf("Dragged by: (%d,%d)\n",x-lastPos[0],y-lastPos[1]);
                            callback(mouseDrag,x,y);
                        }else{
                            lastPos[0]=x;
                            lastPos[1]=y;
                        }
                        isDrag=!isDrag;
                    } else if (button == mouseContext) {
                        //printf("Right click detected at (%d, %d)\n", x, y);
                        callback(mouseContext,x,y);
                    } else if (button == mouseMiddle) {
                        //printf("Middle click detected at (%d, %d)\n", x, y);
                        callback(mouseMiddle,x,y);
                    } else {
                        //printf("Mouse moved: Button=%d, X=%d, Y=%d\n", button, x, y);
                        callback(mouseMove,x,y);
                    }
                    //printf("Task Mouse event: Button=%d, X=%d, Y=%d\n", button, x, y);
                }else if(buf[2]==0x41){
                    //printf("Mouse moved up\n");
                    callback(kbdArrowUp,x,y);
                }else if(buf[2]==0x42){
                    //printf("Mouse moved down\n");
                    callback(kbdArrowDown,x,y);
                }else if(buf[2]==0x43){
                    //printf("Mouse moved right\n");
                    callback(kbdArrowRight,x,y);
                }else if(buf[2]==0x44){
                    //printf("Mouse moved left\n");
                    callback(kbdArrowLeft,x,y);
                }
            }else if(buf[0]==0xa){
                //printf("Enter pressed\n");
                callback(kbdEnter,x,y);
            }else if(buf[0]==0x7f){
                //printf("Backspace pressed\n");
                callback(kbdBackspace,x,y);
            }else{
                callback(otherChars,x,y);
            }
            
        }
    }
    ALLOW_MOUSE=0;
    printf(TRACK_MOUSE_DS);
    /*
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
    */
}

void *task1(void *arg){
    usleep(1e6);
    printf("Task 1\n");
    usleep(1e6);
    return NULL;
}

void *task2(void *arg){
    usleep(1e6/2);
    printf("Task 2\n");
    usleep(1e6/2);
    return NULL;
}

void init(){
    atexit(disableRawMode);
    signal(SIGINT,handleSignal);
    enableRawMode();
    /*
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, task1, NULL);
    pthread_create(&thread2, NULL, task2, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    */
}