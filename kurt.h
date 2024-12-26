#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

enum colors {
    reset=0,
    bold,
    dim,
    underline=4,
    blink,
    invert,
    hidden,
    resetBold=21,
    resetDim,
    resetUnderline,
    resetBlink,
    resetInvert,
    resetHidden,
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

void box(char *str){
    int len=strlen(str);
    //char *buf = (char*)malloc((len+2)*3+1);
    char *str1=(char*)malloc(len+2*3*len+2*3*3+4+1);
    sprintf(str1,"%s%s%s\n%s%s%s\n%s%s%s\n",AS_BOX_UP_LT,AS_BOX_HL,AS_BOX_UP_RT,AS_BOX_VL,str,AS_BOX_VL,AS_BOX_DN_LT,AS_BOX_HL,AS_BOX_DN_RT);
    printf("%s%s%s\n%s%s%s\n%s%s%s\n",AS_BOX_UP_LT_DB,AS_BOX_HL_DB,AS_BOX_UP_RT_DB,AS_BOX_VL_DB,str,AS_BOX_VL_DB,AS_BOX_DN_LT_DB,AS_BOX_HL_DB,AS_BOX_DN_RT_DB);
    //return buf;
    printf("%s",str1);
    int *r=strinf("gdfgdhrdhr\nsgergrrr\ndsggsgr");
    printf("\n%d %d %d\n",r[0],r[1],r[2]);
}

char *box_db(char *str){
    int *r=strinf(str);
    sprintf(str,"",AS_BOX_UP_LT_DB,AS_BOX_HL_DB,AS_BOX_UP_RT_DB,AS_BOX_VL_DB,str,AS_BOX_VL_DB,AS_BOX_DN_LT_DB,AS_BOX_HL_DB,AS_BOX_DN_RT_DB);
    return str;
}

char *color(char *str,unsigned int n,...){
    va_list args;
    va_start(args,n);
    int clr[n];
    for (int i = 0; i < n; i++){
        clr[i] = va_arg(args, int);
    }
    va_end(args);
    int len=strlen(str);
    char *res=(char *)malloc(len+2*n-1+8+1);
    sprintf(res,"\e[%d",clr[0],"\e[0m");
    for(int i=0;i<n;i++){
        res[i+3]=clr[i];
    }
    n+=3;
    res[n++]='m';
    for(int i=0;i<len;i++){
        res[n++]=str[i];
    }
    res[n++]='\\';
    res[n++]='e';
    res[n++]='[';
    res[n++]='0';
    res[n++]='m';
    res[n]='\0';
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

void insert(canvas canv,char *str,int x,int y){
    strInf inf=getStrInf(canv.buf);
    if(x>canv.w||y>canv.h){
        newError("Out of bounds: (%d,%d) in canvas of %dx%d",x,y,canv.w,canv.h);
        return;
    }
    for(int i=0;i<canv.len;i++){
        canv.buf[(y-1)*(inf.cols+1)+x-1+i]=str[i];
    }
}

void display(canvas canv){
    printf("Canvas of %dx%d:\n%s",canv.w,canv.h,canv.buf);
}

void newError(char *str,...){
    va_list args;
    va_start(args,str);
    vfprintf(stderr,str,args);
    va_end(args);
    exit(1);
}