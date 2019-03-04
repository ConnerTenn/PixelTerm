
#ifndef _PIXEL_TERM_H_
#define _PIXEL_TERM_H_


#include <stdio.h>
#include <stdlib.h>
#include <w3mimg.h>
#include <unistd.h>
#include <string>

//#include "w3mimg.h"

/*typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef u_int64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;*/

struct RGB
{
	u_int8_t R;
	u_int8_t G;
	u_int8_t B;
	u_int64_t XColour();
};

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
	
	static Window GetWindow();
	static Display *GetDisplay();

	static void Draw();
	static void Clear();
	static void ForceClear();
	static void DrawPixel(int x, int y, RGB colour);
	static void DrawRectangle(int x, int y, int w, int h, RGB colour);
	static void OutlineRectangle(int x, int y, int w, int h, RGB colour);
	static void DrawLine(int x1, int y1, int x2, int y2, RGB colour);
	static void DrawImage(int x, int y, Image *image);
	static void DrawText(int x, int y, std::string text, RGB colour);

	static int GetWidth();
	static int GetHeight();
};


#endif
