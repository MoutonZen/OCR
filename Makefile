# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -flto
LDFLAGS= -lm
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main


main: main.o Segmentation/pixel_operations.o Segmentation/segmentation.o Image_Treatment/image_treatment.o Network/network.o Build_txt/Build_txt.o $(LDFLAGS)

Build_txt.o: Build_txt.h
network.o: network.h
image_treatment.o: image_treatment.h
segmentation.o: segmentation.h
pixel_operations.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} Segmentation/*.d
	${RM} Segmentation/*.o
	${RM} Segmentation/main
	${RM} Image_Treatment/*.d
	${RM} Image_Treatment/*.o
	${RM} Image_Treatment/main

clean_all:
	${RM} *.o
	${RM} *.d
	${RM} Segmentation/*.d
	${RM} Segmentation/*.o
	${RM} Segmentation/main
	${RM} Image_Treatment/*.d
	${RM} Image_Treatment/*.o
	${RM} Image_Treatment/main
	${RM} image_treatment
	${RM} segmentation
	${RM} main
	
# END