#
# w3mimg/fb/Makefile
#
#

#MGCFLAGS=-I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/libffi-3.2.1/include -pthread -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/uuid 
#OBJS=fb.o fb_img.o 
CFLAGS=$(OPTS) -I ../ -I ../../ -I ../../libwc $(INCLUDE) $(INCLUDE)
CC=gcc

all: fb_w3mimg.o fb.o fb_img.o

fb_w3mimg.o: fb_w3mimg.c
	@echo Compile: $(CC) $@
	@$(CC) $(CFLAGS) -c $<

fb.o: fb.c
	@echo Compile: $(CC) $@
	@$(CC) $(CFLAGS) -c $<

fb_img.o: fb_img.c fb_gdkpixbuf.c fb_imlib2.c
	@echo Compile: $(CC) $@
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f *.o

#


