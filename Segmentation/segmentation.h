#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <stdlib.h>
#include <errx.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int __is_empty_line(SDL_surface* image, height, width);
int __is_empty_column(SDL_surface* image, height, width);
void line_segmentation(SDL_surface* image);
void column_segmentation(SDL_surface* image);

#endif