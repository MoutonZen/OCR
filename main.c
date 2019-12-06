#include "Segmentation/segmentation.h"
#include "Segmentation/pixel_operations.h"
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Image_Treatment/image_treatment.h"
#include "Build_txt/Build_txt.h"
#include "Network/network.h"

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


void train_image(SDL_Surface* image, Network net, char *list, char *output_list)
{
    SDL_LockSurface(image);
    int *res = malloc(sizeof(int) *2);
    float* output = malloc(88*sizeof(char));
    float *binary = malloc(20*20*sizeof(int));
    int width = image->w;
    int height = image->h; 
    int n =0;
    for (int i = 0; i < height; i++)
    {
        int tmp = i;
        for (int j = 0; j < width; j++)
        {
            Uint32 pixel = get_pixel(image, j, i);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if(!(r==255 && g == 0 && b == 0))
            {
                search_end(image, i, j, height, width, res);
                if (res[0]>tmp)
                    tmp = res[0];
                SDL_Surface *letter = SDL_CreateRGBSurface(0, res[1]-j, res[0]-i, 32, 0, 0, 0, 0);
                cut_image(image,letter, i, j, res);
                SDL_Surface *letter_resize = SDL_CreateRGBSurface(SDL_HWSURFACE, 20, 20, letter->format->BitsPerPixel, 0, 0, 0, 0);
                resize(letter, letter_resize);
                to_binary(letter_resize, binary);
                output_network(list, output_list[n], output);
                network_train(net, binary, output);
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
    free(binary);
    free(output);
    SDL_UnlockSurface(image);
}

int* evaluate_image(SDL_Surface* image, Network net, char *list, size_t *length)
{
    *length = 0;
    size_t index_of_space = 0;
    SDL_LockSurface(image);
    int *res = malloc(sizeof(int) *2);
    int *tab_of_space = malloc(sizeof(int)* 1);
    float *binary = malloc(20*20*sizeof(int));
    int width = image->w;
    int height = image->h; 
    size_t size_space = 0;
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
                tab_of_space = realloc(tab_of_space, sizeof(int) *index_of_space+1);
                if (size_space > 10)
                {
                    tab_of_space = realloc(tab_of_space, sizeof(int) *index_of_space+1);
                    tab_of_space[index_of_space] = '\n';
                    index_of_space += 1;
                }
                else
                {
                    tab_of_space = realloc(tab_of_space, sizeof(int) *index_of_space+1);
                    tab_of_space[index_of_space] = ' ';
                    index_of_space += 1;
                }
                size_space = 0;
                search_end(image, i, j, height, width, res);
                if (res[0]>tmp)
                    tmp = res[0];
                SDL_Surface *letter = SDL_CreateRGBSurface(0, res[1]-j, res[0]-i, 32, 0, 0, 0, 0);
                cut_image(image,letter, i, j, res);
                SDL_Surface *letter_resize = SDL_CreateRGBSurface(SDL_HWSURFACE, 20, 20, letter->format->BitsPerPixel, 0, 0, 0, 0);
                resize(letter, letter_resize);
                to_binary(letter_resize, binary);
                network_evaluate(net, binary);
                float tmp = 0;
                int pos = 0;
                for (int i = 0; i < net.Outputs; ++i)
                {
                    if (net.outputs_neur[i]>tmp){
                        tmp = net.outputs_neur[i];
                        pos = i;
                    }
                }
                tab_of_space[index_of_space] = list[pos];
                SDL_FreeSurface(letter);
                SDL_FreeSurface(letter_resize);
                j = res[1]+1;
                tmp = res[0];
                index_of_space += 1;
            }
            if(j >= width-1)
                i = tmp;
        }
    }
    *length = index_of_space;
    SDL_UnlockSurface(image);
    return tab_of_space;
}



int main(int argc, char *argv[])
{
	if (argc != 2 )
	{
		errx(1, "Usage : image file");
	}
    char *list = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	// SDL initialisation
	SDL_Surface* image_surface = NULL;
    SDL_Surface* screen_surface = NULL;
    init_sdl();
    Network *network = network_load();
    Network net;
    if (network == NULL)
    {
        net = __initNetwork__(400, 200, 52);;
    }
    else
        net = *network;
    printf("%d\n", net.Inputs);
    image_surface = load_image(argv[1]);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    // Start of text segmentation

    greyscale(image_surface);
    update_surface(screen_surface, image_surface);
    blackandwhite(image_surface);

    wait_for_keypressed();

    line_segmentation(image_surface);
    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    column_segmentation(image_surface);
    update_surface(screen_surface, image_surface);
    
    wait_for_keypressed();

    size_t *length = malloc(sizeof(size_t));
    *length = 0;
    int *spaces_tab = evaluate_image(image_surface, net, list, length);
    update_surface(screen_surface, image_surface);
    
    wait_for_keypressed();

    printf("%ld\n", *length);
    Build_txt(spaces_tab, *length);

    wait_for_keypressed();


    // RAM release
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
	return 0;
}