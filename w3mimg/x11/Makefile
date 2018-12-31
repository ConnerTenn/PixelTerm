#
# w3mimg/x11/Makefile
#
#

#IMGCFLAGS=-I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/libffi-3.2.1/include -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/uuid -pthread 
#OBJS=x11_w3mimg.o
CFLAGS=$(OPTS) -I ../ -I ../../ -I ../../libwc $(INCLUDE) $(INCLUDE)
CC=gcc

all: x11_w3mimg.o

x11_w3mimg.o: x11_w3mimg.c
	@echo Compile: $(CC) $@
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f *.o


#


