/* $Id: w3mimg.h,v 1.9 2010/12/21 10:13:55 htrb Exp $ */
#ifndef W3MIMG_W3MIMG_H
#define W3MIMG_W3MIMG_H

#include "config.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_W3MIMG_FB
#include "w3mimg/fb/fb.h"
#include "w3mimg/fb/fb_img.h"
#endif

typedef struct {
    void *pixmap;		/* driver specific */
    int width;
    int height;
} W3MImage;

typedef struct _w3mimg_op {
    void *priv;			/* driver specific data */
    int width, height;		/* window width, height */
    int offset_x, offset_y;	/* offset */
    int clear_margin;
    int max_anim;

    int (*init) (struct _w3mimg_op * self);
    int (*finish) (struct _w3mimg_op * self);
    int (*active) (struct _w3mimg_op * self);
    void (*set_background) (struct _w3mimg_op * self, char *background);
    void (*sync) (struct _w3mimg_op * self);
    void (*close) (struct _w3mimg_op * self);

    int (*load_image) (struct _w3mimg_op * self, W3MImage * img, char *fname,
		       int w, int h);
    int (*show_image) (struct _w3mimg_op * self, W3MImage * img,
		       int sx, int sy, int sw, int sh, int x, int y);
    void (*free_image) (struct _w3mimg_op * self, W3MImage * img);
    int (*get_image_size) (struct _w3mimg_op * self, W3MImage * img,
			   char *fname, int *w, int *h);
    int (*clear) (struct _w3mimg_op * self, int x, int y, int w, int h);
} w3mimg_op;

struct x11_info {
	Display *display;
	Window window, parent;
	unsigned long background_pixel;
	GC imageGC;
#if defined(USE_IMLIB)
	ImlibData *id;
#elif defined(USE_GDKPIXBUF)
	int init_flag;
#endif
};

#if defined(USE_GDKPIXBUF)
struct x11_image {
	int total;
	int no;
	int wait;
	int delay;
	Pixmap *pixmap;
};
#endif

#ifdef USE_W3MIMG_X11
extern w3mimg_op *w3mimg_x11open();
#endif
#ifdef USE_W3MIMG_FB
extern w3mimg_op *w3mimg_fbopen();
#endif
#ifdef USE_W3MIMG_WIN
extern w3mimg_op *w3mimg_winopen();
#endif

extern w3mimg_op *w3mimg_open();

#ifdef __cplusplus
}
#endif
#endif /* W3MIMG_W3MIMG_H */
