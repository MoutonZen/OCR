#include "segmentation.h"
#include <errx.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int int main(int argc, char *argv[])
{
	if (argc != 2 )
	{
		errx(1, "Usage : image file")
	}
	// SDL initialisation
	SDL_Surface* image_surface = NULL;
    SDL_Surface* screen_surface = NULL;

    init_sdl();

    image_surface = load_image(argv[1]);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    // Start of text segmentation

    line_segmentation(image_surface);
    update_surface(screen_surface, image_surface);
    
    wait_for_keypressed();
    column_segmentation(image_surface);

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    // RAM release
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
	return 0;
}