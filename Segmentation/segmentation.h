#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <stdlib.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int __is_empty_line(SDL_Surface* image, int height, int width);
int __is_empty_column(SDL_Surface* image, int height, int height_max, int width, int* res);
void line_segmentation(SDL_Surface* image);
void column_segmentation(SDL_Surface* image);

#endif