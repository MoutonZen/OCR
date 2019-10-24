/**
  	SEGMENTATION
 	Description: file containing all that has to do with the segmentation
 	precess
 **/

#include "segmentation.h"
#include <errx.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operation.h"

int __is_empty_line(SDL_surface* image, height, width)
{
	//***************************************//
	//** This function detects empty lines **//
	//***************************************//

	//** returns 0 if it is empty **//
	//** returns 1 otherwise **//
	int res = 0;
	int w = 0
	while(w < width && res == 0)
	{
		Uint32 pixel = get_pixel(image_surface, w, h);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        // If a pixel is not white
        if (r != 255 && g != 255 && b != 255)
        {
        	res = 1;
        }
        w += 1;
	}
	return res;
}

void line_segmentation(SDL_surface* image)
{
	//**************************************************************//
	//** This function switches the lines completely white to red **//
	//**************************************************************//

    // Start of the image scan
    int width = image_surface->w;
    int height = image_surface->h;
    for (int h = 0; h < height; h++)
    {
    	if (__is_empty_line(image_surface, h, width) == 1)
    	{
    		// Changes the pixels to red
    		for (int w = 0; i < width; w++)
    		{
    			Uint32 pixel = get_pixel(image_surface, w, h);
            	Uint8 r, g, b;
            	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            	Uint32 newpixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
            	put_pixel(image_surface, h, w, newpixel);
    		}
    	}
    }
}

int __is_empty_column(SDL_surface* image, height, height_max, width)
{
	//***************************************//
	//** This function detects empty lines **//
	//***************************************//

	// Res[0] is booléen int, 0 -> is white column, 1 -> otherwise
    // Res[1] is the index at the end of the column
	int res[2];
	res[0] = 0
	while (res[0] == 0 && height < height_max)
	{
		Uint32 pixel = get_pixel(image_surface, width, height);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		// If a pixel is not white
        if (r != 255 && g != 255 && b != 255)
        {
        	res[0] = 1;
        }
        height += 1;
	}
	res[1] = height;

	return res;
}

void column_segmentation(SDL_surface* image)
{
	//********************************************************//
	//** This function separates the letters by red columns **//
	//********************************************************//

	// Start of the image scan
	int width = image_surface->w;
    int height = image_surface->h;
    for (int w = 0; i < width; w++)
    {
    	for (int h = 0; i < height; h++)
    	{
    		Uint32 pixel = get_pixel(image_surface, w, h);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (r == 255 && g == 255 && b == 255)
            {
            	// Tab[0] is booléen int, 0 -> is with column, 1 -> otherwise
            	// Tab[1] is the index at the end of the column
            	int tab[2] = __is_empty_column(image, h, height, w);
            	if (tab[0] == 0)
            	{
            		// Changes the pixels to red
            		for (int i = h; i < tab[1]; i++)
            		{
            			Uint32 res_pixel = get_pixel(image_surface, w, i);
            			SDL_GetRGB(red_pixel, image_surface->format, &r, &g, &b);
            			Uint32 newpixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
            			put_pixel(image_surface, i, w, newpixel);
            		}
            	}
            }
    	}
    }
}