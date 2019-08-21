
#ifndef _PIXEL_TERM_H_
#define _PIXEL_TERM_H_


#include <stdio.h>
#include <stdlib.h>
#include <w3mimg.h>
#include <unistd.h>
#include <string>

//#include "w3mimg.h"

typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef u_int64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))
#define ABS(a) ((a)<0?-(a):(a))

struct RGB
{
	u_int8_t R;
	u_int8_t G;
	u_int8_t B;
	u_int64_t XColour();
};

struct ColourVal
{
	double Colour;
	double Scale;
};

#define RED (0.0)
#define YELLOW (1.0/6.0)
#define GREEN (2.0/6.0)
#define CYAN (3.0/6.0)
#define BLUE (4.0/6.0)
#define MAGENTA (5.0/6.0)

//double OppCos(double x);
inline double RedVal(double val)
{
	return val = 2.0-abs(6.0*(0.5-abs(val-0.5))), val=MIN(val,1.0), MAX(val,0.0);
}
inline double GreenVal(double val)
{
	return val = 2.0-abs(6.0*val-2.0), val=MIN(val,1.0), MAX(val,0.0);
}
inline double BlueVal(double val)
{
	return val = 2.0-abs(6.0*val-4.0), val=MIN(val,1.0), MAX(val,0.0);
}
inline RGB RGBVal(ColourVal val)
{		
	return RGB{(u8)(255.0*RedVal(val.Colour)*val.Scale),(u8)(255.0*GreenVal(val.Colour)*val.Scale),(u8)(255.0*BlueVal(val.Colour)*val.Scale)};
}






class PixelTerm;
class Image;

class Image
{
	friend PixelTerm;
private:
	XImage *Ximage;
	u_int32_t *Raw;
	int Width, Height;
public:
	Image(int width, int height);
//	Image(const Image &other);
	~Image();

	void Pixel(int x, int y, RGB colour);
};

class PixelTerm
{
	friend Image;
private:
	static x11_info *Xi;
	static w3mimg_op *W_op;
	static int Width, Height;

public:
	static bool Init();
	static void Close();

	static void Draw();
	static void Clear();
	static void ForceClear();
	static void DrawPixel(int x, int y, RGB colour);
	static void DrawRectangle(int x, int y, int w, int h, RGB colour);
	static void DrawLine(int x1, int y1, int x2, int y2, RGB colour);
	static void DrawImage(int x, int y, Image *image);
	static void DrawText(int x, int y, std::string text, RGB colour);

	static int GetWidth();
	static int GetHeight();
};



void reset_terminal_mode();

void set_conio_terminal_mode();

int kbhit();

int getch();

#endif
