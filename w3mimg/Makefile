#
# w3mimg/Makefile
#



INCLUDE=-I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/libffi-3.2.1/include -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/uuid
#OBJS=w3mimg.o x11/x11_w3mimg.o fb/fb_w3mimg.o fb/fb.o fb/fb_img.o
CFLAGS=$(OPTS) -I ../ -I ../libwc $(INCLUDE) $(INCLUDE)
CC=gcc

SUBDIRS=fb x11


all: w3mimg.o .PHONY
.PHONY: $(SUBDIRS)

w3mimg.o: w3mimg.c
	@echo Compile: $(CC) $@
	@$(CC) $(CFLAGS) -c $<

$(SUBDIRS):
	@cd $@ && $(MAKE) OPTS="$(OPTS)" INCLUDE="$(INCLUDE)"

clean:
	@rm -f *.o
	@for dir in $(SUBDIRS); do \
		(cd $$dir && $(MAKE) clean); \
	done



