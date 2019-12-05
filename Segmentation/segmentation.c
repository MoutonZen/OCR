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

// Prototype
void rm_wrong_space(space_list *sentinelle);


int __is_empty_line(SDL_Surface* image, int height, int width_max)
{
	//***************************************//
	//** This function detects empty lines **//
	//***************************************//

	SDL_LockSurface (image);
	//** returns 0 if it is empty **//
	//** returns 1 otherwise **//
	int res = 0;
	int w = 0;
	while(w < width_max && res == 0)
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
	SDL_UnlockSurface(image);
	return res;
}

void line_segmentation(SDL_Surface* image)
{
	//**************************************************************//
	//** This function switches the lines completely white to red **//
	//**************************************************************//

	SDL_LockSurface (image);
    // Start of the image scan
    int width = image->w;
    int height = image->h;
    for (int i = 0; i < height; i++)
    {
    	if (__is_empty_line(image, i, width) == 0)
    	{
    		// Changes the pixels to red
    		for (int j = 0; j < width; j++)
    		{
            	Uint8 r, g, b;
            	r = 255;
            	g = 0;
            	b = 0;
            	Uint32 newpixel = SDL_MapRGB(image->format, r, g, b);
            	put_pixel(image, j, i, newpixel);
    		}
    	}
    }
    SDL_UnlockSurface(image);
}

int* __is_empty_column(SDL_Surface* image, int height, int height_max, int width, int* res)
{
	//***************************************//
	//** This function detects empty lines **//
	//***************************************//

	SDL_LockSurface (image);
	// Res[0] is booléen int, 0 -> is white column, 1 -> otherwise
    // Res[1] is the index at the end of the column
	res[0] = 0;
	int tst = 1;
	while (res[0] == 0 && height < height_max && tst)
	{
		Uint32 pixel = get_pixel(image, width, height);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
		// If a pixel is not white
        if (r != 255 && g != 255 && b != 255)
        {
        	res[0] = 1;
        }
        if (r == 255 && g ==0 && b == 0)
        {
        	tst = 0;
        }
        height += 1;
	}
	res[1] = height;
	SDL_UnlockSurface(image);
	return res;
}

void column_segmentation(SDL_Surface* image)
{
	//********************************************************//
	//** This function separates the letters by red columns **//
	//********************************************************//

	SDL_LockSurface (image);
    // Start of the image scan
    int width = image->w;
    int height = image->h;

    // Tab[0] is booléen int, 0 -> is with column, 1 -> otherwise
    // Tab[1] is the index at the end of the column
    int* tab = malloc(sizeof(int) *2);

    for (int i = 0; i < height; ++i)
    {
        Uint32 pixel = get_pixel(image, 0, i);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
        if (!(r==255 && g ==0 && b ==0))
        {
            for (int j = 0; j < width; ++j)
            {
                Uint32 pixel = get_pixel(image, j, i);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image->format, &r, &g, &b);
                if (r == 255 && g == 255 && b == 255)
                {
                    tab = __is_empty_column(image, i, height, j, tab);
                    if (tab[0] == 0)
                    {
                        // Changes the pixels to red
                        for (int k = i; k < tab[1]; k++)
                        {
                            Uint32 res_pixel = get_pixel(image, j, k);
                            SDL_GetRGB(res_pixel, image->format, &r, &g, &b);
                            Uint32 newpixel = SDL_MapRGB(image->format, 255, 0, 0);
                            put_pixel(image, j, k, newpixel);
                        }
                    }
                }
            }
        }
    }
    free(tab);
    SDL_UnlockSurface(image);
}


void cut_image(SDL_Surface *img, SDL_Surface *letter, int i, int j, int* res)
{
    //**********************************************//
    //** This function copy a letter in new image **//
    //**********************************************//
    for (int k = 0; i+k < res[0]; ++k)
    {
        for (int n = 0; j+n < res[1]; ++n)
        {
            Uint32 pixel = get_pixel(img, j+n, i+k);
            put_pixel(letter, n, k, pixel);
        }
    }
}

void search_end(SDL_Surface* image, int i, int j, int height, int width, int *res)
{
    SDL_LockSurface (image);
    int k = i;
    for (; k < height; ++k)
    {
        Uint32 pixel = get_pixel(image, j, k);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
        if(r==255 && g == 0 && b == 0)
        {
            break;
        }
    }
    res[0] = k;
    k = j;
    for (; k < width; ++k)
    {
        Uint32 pixel = get_pixel(image, k, i);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image->format, &r, &g, &b);
        if(r==255 && g == 0 && b == 0)
        {
            break;
        }
    }
    res[1] = k;
    SDL_UnlockSurface(image);
}

SDL_Surface* resize(SDL_Surface *img, SDL_Surface *dst)
{
    SDL_SoftStretch(img, NULL, dst, NULL);
    return dst; 
}

int* separate_caractere(SDL_Surface* image)
{
    SDL_LockSurface(image);
    int *res = malloc(sizeof(int) *2);
    int *tab_of_space = malloc(sizeof(int)* 1);
    int width = image->w;
    int height = image->h; 
    size_t size_space = 0;
    size_t index_of_space = 0;
    for (int i = 0; i < height; i++)
    {
        int tmp = i;
        for (int j = 0; j < width; j++)
        {
        	size_space += 1;
            Uint32 pixel = get_pixel(image, j, i);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if(!(r==255 && g == 0 && b == 0))
            {
            	tab_of_space = realloc(tab_of_space, sizeof(int)*index_of_space+10);
            	if (size_space > 10)
            	{
            		tab_of_space[index_of_space] = '\n';
            	}
            	else if (size_space < 3)
            	{
            		tab_of_space[index_of_space] = '-';
            	}
            	else
            	{
            		tab_of_space[index_of_space] = ' ';
            	}
            	index_of_space += 1;
            	size_space = 0;
                search_end(image, i, j, height, width, res);
                if (res[0]>tmp)
                    tmp = res[0];
                SDL_Surface *letter = SDL_CreateRGBSurface(0, res[1]-j, res[0]-i, 32, 0, 0, 0, 0);
                cut_image(image,letter, i, j, res);
                SDL_Surface *letter_resize = SDL_CreateRGBSurface(SDL_HWSURFACE, 25, 25, letter->format->BitsPerPixel, 0, 0, 0, 0);
                resize(letter, letter_resize);
                SDL_FreeSurface(letter);
                SDL_FreeSurface(letter_resize);
                j = res[1]+1;
                tmp = res[0];
            }
            if(j >= width-1)
                i = tmp;
        }
    }
    free(res);
    SDL_UnlockSurface(image);
    return tab_of_space;
}


