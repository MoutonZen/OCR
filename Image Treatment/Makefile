# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: image_treatment grayscale

image_treatment: pixel_operations.o image_treatment.o
grayscale: pixel_operations.o grayscale.o

grayscale.o: pixel_operations.h
pixel_operations.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} image_treatment
	${RM} grayscale
	${RM} character.txt

# END