#ifndef __FBDRAW_H__
#define __FBDRAW_H__

#include"common.hpp"

#define FB_OPEN_FAIL 1
#define FB_GET_FINFO_FAIL 2
#define FB_GET_VINFO_FAIL 3
#define FB_MMAP_FAIL 4
#define RADIUS 20

typedef unsigned char uchar;

extern dev_fb fb;

//프래임 버퍼에 접근하고 초기화하는 함수
int fb_init(dev_fb* fb);

//x y 좌표 값을 받아 pixel 객체를 생성하는 함수
pixel fb_toPixel(int x,int y);


//pixel객체에 x,y 좌표에 b g r 순으로 메모리에 값을 할당하는 함수
void fb_drawPixelPx(dev_fb* fb, pixel px, char r, char g, char b);

//x,y 좌표에 b g r 순으로 메모리에 값을 할당하는 함수
void fb_drawPixel(dev_fb* fb, int x, int y, char r, char g, char b);

//x,y 좌표에 b g r a순으로 메모리에 값을 할당하는 함수
void fb_drawPixelwithAlpha(dev_fb* fb, int x, int y, char r, char g, char b, char a);

//프래임버퍼에서 출력하는 전체 공간에 b g r 데이터를 할당하는 함수
void fb_fillScr(dev_fb* fb, char r, char g, char b);


//전달 받은 pixel에서 시작하여 높이 h 넓이 w인 사각형을 만듭니다.(색상: rgb에 해당하는 색상)
void fb_drawBox(dev_fb* fb, pixel px, int w, int h, char r, char g, char b);

void fb_drawBoxWidthAlpa(dev_fb* fb, pixel px, int w, int h, char r, char g, char b, char a);

//전달 받은 pixel에서 시작하여 높이 h 넓이 w인 사각형을 만듭니다.(색상: rgb에 해당하는 색상, 투명도: a)
void fb_fillBox(dev_fb* fb, pixel px, int w, int h, char r, char g, char b);

//시작점(start)부터 끝점(end)까지 선을 긋습니다.(색상: rgb에 해당하는 색상)
void fb_drawLine(dev_fb* fb, pixel start, pixel end, char r, char g, char b);


//숫자 알파벳 기호에 한하여 입력받은 문자를 프레임버퍼에 그려주는 함수
void fb_drawChar(dev_fb* fb, char c, pixel start, short height, char r, char g, char b);

//숫자 알파벳 기호에 한하여 입력받은 문자를 프레임버퍼에 그려주는 함수
void fb_printStr(dev_fb* fb, const char* str, pixel* cursor, short height, char r, char g, char b);

//입력 받은 문자열을 fb_drawChar()함수를 활용하여 프레임버퍼에 그려주는 함수
void fb_drawFilledCircle(dev_fb* fb, pixel center,char r, char g, char b);


//프래임버퍼를 사용하기 위한 메모리 맵핑을 해제해주는 함수
void fb_close(dev_fb* fb);


#endif//__FBDRAW_H__