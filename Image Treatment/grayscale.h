#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include <stdlib.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void grayscale(SDL_Surface* image_surface);
SDL_Surface* resize(SDL_Surface *img);

#endif
