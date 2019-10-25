/**
  	SEGMENTATION
 	Description: file containing all that has to do with the segmentation
 	precess
 **/

#include "segmentation.h"
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

int __is_empty_line(SDL_Surface* image, int height, int width)
{
	//***************************************//
	//** This function detects empty lines **//
	//***************************************//

	//** returns 0 if it is empty **//
	//** returns 1 otherwise **//
	int res = 0;
	int w = 0;
	while(w < width && res == 0)
	{
		Uint32 pixel = get_pixel(image, w, height);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
        // If a pixel is not white
        if (r != 255 && g != 255 && b != 255)
        {
        	res = 1;
        }
        w += 1;
	}
	return res;
}

void line_segmentation(SDL_Surface* image)
{
	//**************************************************************//
	//** This function switches the lines completely white to red **//
	//**************************************************************//

    // Start of the image scan
    int width = image->w;
    int height = image->h;
    for (int i = 0; i < height; i++)
    {
    	if (__is_empty_line(image, i, width) == 1)
    	{
    		// Changes the pixels to red
    		for (int j = 0; j < width; j++)
    		{
    			Uint32 pixel = get_pixel(image, j, i);
            	Uint8 r, g, b;
            	SDL_GetRGB(pixel, image->format, &r, &g, &b);
            	Uint32 newpixel = SDL_MapRGB(image->format, 255, 0, 0);
            	put_pixel(image, i, j, newpixel);
    		}
    	}
    }
}

int __is_empty_column(SDL_Surface* image, int height, int height_max, int width, int* res)
{
	//***************************************//
	//** This function detects empty lines **//
	//***************************************//

	// Res[0] is booléen int, 0 -> is white column, 1 -> otherwise
    // Res[1] is the index at the end of the column
	res[0] = 0;
	while (res[0] == 0 && height < height_max)
	{
		Uint32 pixel = get_pixel(image, width, height);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
		// If a pixel is not white
        if (r != 255 && g != 255 && b != 255)
        {
        	res[0] = 1;
        }
        height += 1;
	}
	res[1] = height;

	return *res;
}

void column_segmentation(SDL_Surface* image)
{
	//********************************************************//
	//** This function separates the letters by red columns **//
	//********************************************************//

	// Start of the image scan
	int width = image->w;
    int height = image->h;
    for (int w = 0; w < width; w++)
    {
    	for (int h = 0; h < height; h++)
    	{
    		Uint32 pixel = get_pixel(image, w, h);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r == 255 && g == 255 && b == 255)
            {
            	// Tab[0] is booléen int, 0 -> is with column, 1 -> otherwise
            	// Tab[1] is the index at the end of the column
                int* tab = malloc(2);
            	__is_empty_column(image, h, height, w, tab);
            	if (tab[0] == 0)
            	{
            		// Changes the pixels to red
            		for (int i = h; i < tab[1]; i++)
            		{
            			Uint32 res_pixel = get_pixel(image, w, i);
            			SDL_GetRGB(res_pixel, image->format, &r, &g, &b);
            			Uint32 newpixel = SDL_MapRGB(image->format, 255, 0, 0);
            			put_pixel(image, i, w, newpixel);
            		}
            	}
            }
    	}
    }
}