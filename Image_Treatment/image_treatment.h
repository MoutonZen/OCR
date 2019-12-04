#ifndef IMAGE_TREATMENT_H
#define IMAGE_TREATMENT_H

void contrast(SDL_Surface* img);
void greyscale(SDL_Surface *img);
void blackandwhite(SDL_Surface *img);
void binarisation();
SDL_Surface* resize(SDL_Surface *img);


#endif  