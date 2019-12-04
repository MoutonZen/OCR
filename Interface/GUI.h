# ifndef GUI_H_
# define GUI_H_

# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <gtk/gtk.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>


typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;

#define UNUSED(x) (void)(x)

void save_text(GtkButton *button, GtkTextBuffer *buffer);

void load_images(GtkButton *button, GtkImage *image);

void open_image(GtkButton *button, GtkLabel *text_label);

int launchOCR(GtkButton *button, GtkTextBuffer *buffer);

void create_window(int argc, char *argv[]);

void trainOCR();

void quitOCR();

# endif

