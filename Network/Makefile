CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD -g
CFLAGS= -Wall -Wextra -Werror -std=c99 -g
LDFLAGS= -lm
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main

main: main.o network.o $(LDFLAGS)

network.o: network.h

clean:
	${RM} *.o
	${RM} *.d