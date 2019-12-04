# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main


main: main.o Segmentation/pixel_operations.o Segmentation/segmentation.o Image_Treatment/image_treatment.o

image_treatment.o: image_treatment.h
segmentation.o: segmentation.h
pixel_operations.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} image_treatment
	${RM} segmentation
	${RM} main

# END