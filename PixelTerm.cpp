
#include "PixelTerm.h"
#include <math.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

//RGB::RGB() { }
//RGB::RGB(u64 data) { Pixel = data; }
//RGB::RGB(u8 r, u8 g, u8 b) { R=r; G=g; B=b; }
u_int32_t RGB::XColour()
{
	return (B<<8*0) | (G<<8*1) | (R<<8*2);// | (0x00<<8*3);
}

unsigned char *PixelTerm::FB0=0, *PixelTerm::FB1=0;
int PixelTerm::Width=0, PixelTerm::Height=0, PixelTerm::FBsize=0;


bool PixelTerm::Init()
{
	int sizefd = open("/sys/class/graphics/fb0/virtual_size", O_RDONLY);
	char sizebuff[32];
	read(sizefd, sizebuff, 32);
	Width=atoi(sizebuff);
	Height=atoi(sizebuff+(int)log10(Width)+2);
	FBsize=Width*Height*4;
	close(sizefd);
	printf("Width:%d Height:%d Size:%d\n", Width, Height, FBsize);

	int dispfd = open("/dev/fb0", O_RDWR);
	printf("Display File Descriptor: %d\n", dispfd);

	FB0 = (unsigned char *)mmap(0, FBsize, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED,dispfd,0);
	printf("mmap Address: %p\n", FB0);
	if (FB0==MAP_FAILED) { perror("Error: mmap failed: "); return false; }
	close(dispfd);

	FB1=(unsigned char *)malloc(FBsize);
	
	return true;
}
void PixelTerm::Close()
{
	free(FB1);
	munmap(FB0, FBsize);
}

void PixelTerm::Draw()
{
	memcpy(FB0, FB1, FBsize);
}
void PixelTerm::Clear()
{
	
}
void PixelTerm::ForceClear()
{
	memset(FB1, 0, FBsize);
}
void PixelTerm::DrawPixel(int x, int y, RGB colour)
{
	
}
void PixelTerm::DrawRectangle(int x, int y, int w, int h, RGB colour)
{

}
void PixelTerm::DrawLine(int x0, int y0, int x1, int y1, RGB colour)
{
	//int x, y;
	//if (x1>x2) { x=x1; x1=x2; x2=x; }
	//if (y1>y2) { y=y1; y1=y2; y2=y; }
	int dx = abs(x1-x0), dy = -abs(y1-y0);
	int sx = (x0<x1 ? 1 : -1), sy = (y0<y1 ? 1 : -1);
	int err = dx+dy;
	
	//printf("%d,%d -> %d,%d\n",x0,y0,x1,y1);
	while(true)
	{
		if (x0==x1 && y0==y1) { break; }
		int e2 = 2*err;
		if (e2 >= dy) { err+=dy; x0+=sx; }
		if (e2 <= dx) { err+=dx; y0+=sy; }
		//printf("%d %d\n", x0, y0);
		*(u_int32_t *)(FB1 + y0*Width*4 + x0*4) = colour.XColour();
	}
	//printf("\n");
}
// void PixelTerm::DrawText(int x, int y, std::string text, RGB colour)
// {
//	
//	//
//	XGCValues values;
//	values.foreground = colour.XColour();
//	//values.font = font->fid;
//	unsigned long mask = GCForeground;// | GCFont;
//	GC gc = XCreateGC(Xi->display, Xi->window, mask, &values);
//	//XFontStruct *font = XQueryFont(Xi->display, XGContextFromGC(gc));
//	//XTextItem textItem;
//	//textItem.chars = (char *)text.c_str();
//	//textItem.nchars = text.length();
//	//textItem.font = font->fid;
//	//XDrawText(Xi->display, Xi->window, gc, x, y, &textItem, 1);
//	//XFreeFontInfo(0, font, 1);
//	//XDrawImageString(Xi->display, Xi->window, gc, x, y, text.c_str(), text.length());
//	XDrawString(Xi->display, Xi->window, gc, x, y, text.c_str(), text.length());
//	XFreeGC(Xi->display, gc);
// }

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

