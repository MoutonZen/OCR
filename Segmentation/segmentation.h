#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <stdlib.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


typedef struct list
{
	struct list *next;
	int px_largeur;
	int posx_space;
}space_list;

int __is_empty_line(SDL_Surface* image, int height, int width);
int* __is_empty_column(SDL_Surface* image, int height, int height_max, int width, int* res);
void line_segmentation(SDL_Surface* image);
void column_segmentation(SDL_Surface* image);
void search_end(SDL_Surface* image, int i, int j, int height, int width, int *res);
void cut_image(SDL_Surface *img, SDL_Surface *letter, int i, int j, int* res);
SDL_Surface* resize(SDL_Surface *img, SDL_Surface *dst);
void output_network(char *outputlist, char caratere, float *output);
void to_binary(SDL_Surface *img, float *res);

/*
space_list* Search_space(SDL_Surface* image, int h);
int list_is_empty(struct list *list);
void list_insert(struct list *list, struct list elm);
void rm_wrong_space(space_list *sentinelle);
*/

#endif
