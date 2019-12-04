#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "image_treatment.h"



void contrast(SDL_Surface* img)
{
	

	Uint8 r, g, b;
	int height = img->h;
	int width = img->w;
	int intensite_max = 255;
	int histogram[256];

	for (int i = 0; i <= intensite_max; i++)
		histogram[i] = 0;

	// We build the histogram
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Uint32 pixel = get_pixel(img, i, j);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);

			histogram[(int)r]++;
		}
	}
}


void greyscale(SDL_Surface *img)
{

	Uint8 r, g, b;
	int width = img->w;
	int height = img->h;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Uint32 pixel = get_pixel(img, i, j);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			Uint8 average = 0.3*r + 0.59*g + 0.11*b;
			r = g = b = average;
			Uint32 updated_pixel = SDL_MapRGB(img->format, r, g, b);
			put_pixel(img, i, j, updated_pixel);
		}
	}
}

void blackandwhite(SDL_Surface *img)
{

	int width = img->w;
	int height = img->h;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Uint32 pixel = get_pixel(img, i, j);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			Uint8 average = 0.3*r + 0.59*g + 0.11*b;
			r = g = b = average;
			if (r >= 127 && g >= 127 && b >= 127)
			{
				r = 255;
				g = 255;
				b = 255;
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;
			}

			Uint32 updated_pixel = SDL_MapRGB(img->format, r, g, b);
			put_pixel(img, i, j, updated_pixel);
		}
	}

}

void binarisation()
{
	int array[625];
	FILE * fichier;
	char output[] = "character.txt";
	fichier = fopen(output, "w");

	for (int a = 0; a < 625; a++) 
	{
		if (a % 25 == 24) {
			fprintf(fichier, (array[a] > 120) ? " " : "@");
			fprintf(fichier, "\n");
		}
		else {
			fprintf(fichier, (array[a] > 120) ? " " : "@");

		}

	}
	fclose(fichier);
}

SDL_Surface* resize(SDL_Surface *img)
{
	int w=200;
	int h=200;
	SDL_Surface *dst= SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, img->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_SoftStretch(img, NULL, dst, NULL);
	return dst; 
	
}