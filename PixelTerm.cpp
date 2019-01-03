
#include "PixelTerm.h"

//RGB::RGB() { }
//RGB::RGB(u64 data) { Pixel = data; }
//RGB::RGB(u8 r, u8 g, u8 b) { R=r; G=g; B=b; }
u_int64_t RGB::XColour()
{
	return (B<<8*0) | (G<<8*1) | (R<<8*2) | (0xff<<8*3);
}

x11_info *PixelTerm::Xi = 0;
w3mimg_op *PixelTerm::W_op = 0;
int PixelTerm::Width=0, PixelTerm::Height=0;

Image::Image(int width, int height)
{
	XWindowAttributes attr;
	XGetWindowAttributes(PixelTerm::Xi->display, PixelTerm::Xi->window, &attr);
	XVisualInfo info;
	XMatchVisualInfo(PixelTerm::Xi->display, 0, attr.depth, TrueColor, &info);
	u_int32_t *data = new u_int32_t[width*height];
	Raw = data;
	Ximage = XCreateImage(PixelTerm::Xi->display, info.visual, attr.depth, XYPixmap, 0, (char *)data, width, height, 32, 0);
	XInitImage(Ximage);
}
/*Image::Image(const Image &other)
{
	printf("Copy Image\n");
	Ximage = other.Ximage;
	Width = other.Width;
	Height = other.Height;
}*/
Image::~Image()
{
	XDestroyImage(Ximage);
}
void Image::Pixel(int x, int y, RGB colour)
{
	XPutPixel(Ximage, x, y, colour.XColour());
	//Raw[y*] = 0xffffffff;//colour.XColour();
}

bool PixelTerm::Init()
{
	W_op = w3mimg_open();
	if (!W_op) { return false; }
	W_op->offset_x = 0;
	W_op->offset_y = 0;
	W_op->max_anim = 100;
	W_op->clear_margin = 0;

	Width=W_op->width;
	Height=W_op->height;
	printf("%dx%d\n",Width,Height);

	if (!W_op->init(W_op)) { return false; }
	W_op->set_background(W_op, 0);

	Xi = (x11_info *)W_op->priv;

	return true;
}
void PixelTerm::Close()
{
	W_op->close(W_op);
}

void PixelTerm::Draw()
{
	W_op->sync(W_op);
}
void PixelTerm::Clear()
{
	W_op->clear(W_op, 0, 0, 0, 0);
}
void PixelTerm::ForceClear()
{
	DrawRectangle(0,0,Width,Height, {0,0,0});
}
void PixelTerm::DrawPixel(int x, int y, RGB colour)
{
	XGCValues values;
	values.foreground = colour.XColour();
	unsigned long mask = GCForeground;
	GC gc = XCreateGC(Xi->display, Xi->window, mask, &values);
	XDrawPoint(Xi->display, Xi->window, gc, x, y);
	XFreeGC(Xi->display, gc);
}
void PixelTerm::DrawRectangle(int x, int y, int w, int h, RGB colour)
{
	XGCValues values;
	values.foreground = colour.XColour();
	unsigned long mask = GCForeground;
	GC gc = XCreateGC(Xi->display, Xi->window, mask, &values);
	XFillRectangle(Xi->display, Xi->window, gc, x, y, w, h);
	XFreeGC(Xi->display, gc);
}
void PixelTerm::DrawLine(int x1, int y1, int x2, int y2, RGB colour)
{
	XGCValues values;
	values.foreground = colour.XColour();
	unsigned long mask = GCForeground;
	GC gc = XCreateGC(Xi->display, Xi->window, mask, &values);
	XDrawLine(Xi->display, Xi->window, gc, x1, y1, x2, y2);
	XFreeGC(Xi->display, gc);
}
void PixelTerm::DrawImage(int x, int y, Image *image)
{
	XGCValues values;
	values.foreground = 0xffffff;
	values.background = 0xffffff;
	unsigned long mask = GCForeground | GCBackground;
	GC gc = XCreateGC(Xi->display, Xi->window, mask, &values);
	XPutImage(Xi->display, Xi->window, gc, image->Ximage, 0, 0, x, y, image->Width, image->Height);
	XFreeGC(Xi->display, gc);
}
void PixelTerm::DrawText(int x, int y, std::string text, RGB colour)
{
	
	//
	XGCValues values;
	values.foreground = colour.XColour();
	//values.font = font->fid;
	unsigned long mask = GCForeground;// | GCFont;
	GC gc = XCreateGC(Xi->display, Xi->window, mask, &values);
	//XFontStruct *font = XQueryFont(Xi->display, XGContextFromGC(gc));
	//XTextItem textItem;
	//textItem.chars = (char *)text.c_str();
	//textItem.nchars = text.length();
	//textItem.font = font->fid;
	//XDrawText(Xi->display, Xi->window, gc, x, y, &textItem, 1);
	//XFreeFontInfo(0, font, 1);
	//XDrawImageString(Xi->display, Xi->window, gc, x, y, text.c_str(), text.length());
	XDrawString(Xi->display, Xi->window, gc, x, y, text.c_str(), text.length());
	XFreeGC(Xi->display, gc);
}

int PixelTerm::GetWidth() { return Width; }
int PixelTerm::GetHeight() { return Height; }