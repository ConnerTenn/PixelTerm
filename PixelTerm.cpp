
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




#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>

struct termios orig_termios;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
	struct termios new_termios;

	// take two copies - one for now, one for later
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	// register cleanup handler, and set the new terminal mode
	atexit(reset_terminal_mode);
	//cfmakeraw(&new_termios);
	//new_termios.c_oflag &= ~OPOST;
	new_termios.c_lflag &= ~(ECHO | ECHONL| ICANON | IEXTEN);
	
	//new_termios.c_oflag |= ONLCR;
	//new_termios.c_lflag |= ISIG;
	tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) { return r; }
	else { return c; }
}