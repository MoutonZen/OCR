#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

// TODO: Insert all the above functions.
void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}
SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}


void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}


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

void SDL_FreeSurface(SDL_Surface *surface);


int main()
{
    init_sdl();
    SDL_Surface* img = load_image("MyimageTreatement.jpg");

    int width = img->w;
    int height = img->h;
	int array[625];


    for(int i = 0; i < height; i++){
	    for(int j = 0; j < width; j++){
		    Uint32 pixel = get_pixel(img, i, j);
		    Uint8 r, g, b;
		    SDL_GetRGB(pixel, img->format, &r, &g, &b);
		    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
		    r = g = b = average;
			array[j*height + i] = average;
	    }
    }

	FILE * fichier;
	char output[] = "character.txt";
	fichier = fopen(output, "w");

	for (int a = 0; a < 625; a++) {
		if (a % 25 == 24) {
			fprintf(fichier, (array[a] > 120) ? " " : "@");
			fprintf(fichier, "\n");
		}
		else {
			fprintf(fichier, (array[a] > 120) ? " " : "@");

		}

	}
	printf("\n");
	fclose(fichier);
   

    
}

