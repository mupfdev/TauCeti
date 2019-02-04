PROJECT=TauCeti

ifeq ($(OS),Windows_NT)
	OUT=$(PROJECT).exe
	TOOLCHAIN=i686-w64-mingw32
	CC=$(TOOLCHAIN)-cc
	LIBESZFW=external/eszFW/lib/libeszFW.lib
	LIBTMX=external/eszFW/lib/libtmx.lib
else
	OUT=$(PROJECT)
	TOOLCHAIN=local
	UNAME_S := $(shell uname -s)
	LIBESZFW=external/eszFW/lib/libeszFW.a
	LIBTMX=external/eszFW/lib/libtmx.a
endif

LIBS=\
	-lSDL2\
	-lSDL2_image\
	-lSDL2_mixer\
	-lSDL2_ttf\
	-lxml2 -lz -llzma -lm

CFLAGS=\
	-D_REENTRANT\
	-DWANT_ZLIB\
	-Iexternal/eszFW/src\
	-Iexternal/eszFW/external/tmx/src\
	-isystem /usr/$(TOOLCHAIN)/include/libxml2\
	-isystem /usr/$(TOOLCHAIN)/include/SDL2\
	-O2\
	-pedantic-errors\
	-std=c99\
	-Wall\
	-Werror\
	-Wextra

SRCS=\
	$(wildcard src/*.c)

OBJS=$(patsubst %.c, %.o, $(SRCS))
