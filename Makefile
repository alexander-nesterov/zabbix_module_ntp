NAME_LIBRARY=ntp.so
ROOT_SOURCES=src
SOURCES=$(ROOT_SOURCES)/module.c $(ROOT_SOURCES)/ntp.c
ZBX_INCLUDE=../../../include
CFLAGS=-m64 -fPIC -shared -Wall

all:
	gcc $(CFLAGS) $(LDFLAGS) -o $(NAME_LIBRARY) $(SOURCES) -I$(ZBX_INCLUDE)
clean:
	rm -rf *.so