#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void grayscale(SDL_Surface* image_surface)
{
    int width = image_surface->w;
    int height = image_surface->h;

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
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
            
            Uint32 updated_pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, i, j, updated_pixel);
        }
    }
}
