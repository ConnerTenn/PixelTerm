
SUBDIRS=w3mimg

INCLUDE=-I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/libffi-3.2.1/include -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/uuid
OBJS=PixelTerm.o w3mimg/w3mimg.o w3mimg/x11/x11_w3mimg.o w3mimg/fb/fb_w3mimg.o w3mimg/fb/fb.o w3mimg/fb/fb_img.o
OPTS=-g -I ./
LINK=-pthread -lgdk_pixbuf_xlib-2.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype  -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lX11
CFLAGS=$(OPTS) -I w3mimg $(INCLUDE)
CC=g++

force:

all: PixelTerm.o .PHONY libPixelTerm.a example
.PHONY: $(SUBDIRS)

example: example.cpp Projection.h libPixelTerm.a
	@echo Compile: $(CC) $@
	@$(CC) $(CFLAGS) $< libPixelTerm.a $(LINK) $(LINK) -o $@

libPixelTerm.a: $(OBJS)
	@echo Creating Library: $@
	@ar rcs $@ $(OBJS)

PixelTerm.o: PixelTerm.cpp PixelTerm.h
	@echo Compile: $(CC) $@
	@$(CC) $(CFLAGS) -c $<

$(SUBDIRS):
	@cd $@ && $(MAKE) OPTS="$(OPTS)" INCLUDE="$(INCLUDE)"

clean:
	@rm -f *.o *.a example
	@for dir in $(SUBDIRS); do \
		(cd $$dir && $(MAKE) clean); \
	done

force: clean all
