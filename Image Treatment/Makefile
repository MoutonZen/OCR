# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main


main: main.o pixel_operations.o image_treatment.o

image_treatment.o: image_treatment.h
pixel_operations.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} image_treatment
	${RM} main
	${RM} character.txt

# END