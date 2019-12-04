#include "GUI.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

gchar *filename = "";
char *text = "";
GtkWidget *parent;
void save_text(GtkButton *button, GtkTextBuffer *buffer)
{
  UNUSED(button);
  UNUSED(buffer);
  GtkWidget *dialog;
  GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
  dialog = gtk_file_chooser_dialog_new ("Save Text ",
                    GTK_WINDOW (toplevel),
                    GTK_FILE_CHOOSER_ACTION_SAVE,
                    "Cancel", GTK_RESPONSE_CANCEL,
                    "Save", GTK_RESPONSE_ACCEPT,
                    NULL);
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    /* set the contents of the file to the text from the buffer */
    if (filename != NULL)
      g_file_set_contents (filename, text, strlen(text), NULL);
    else
      g_file_set_contents (filename, text, strlen(text), NULL);
  }
  gtk_widget_destroy (dialog);

}

void load_images(GtkButton *button, GtkImage *image)
{
  if(strcmp(filename,"") == 0)
  {
	  return;
  }
  UNUSED(button);
  //SDL_Surface *img = IMG_Load((char *)filename);
  gtk_image_set_from_file (GTK_IMAGE (image), filename);
  
}

void open_image(GtkButton *button, GtkLabel *text_label)
{
  GtkWidget *label = (GtkWidget *) text_label;
	GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
	                                                GTK_WINDOW (toplevel),
	                                                GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                "Open", GTK_RESPONSE_ACCEPT,
	                                                "Cancel", GTK_RESPONSE_CANCEL,
	                                                NULL);
	                                                
	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
		{
			filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      gtk_label_set_text(GTK_LABEL(label),filename);
			break;
		}
		default:
			break;
	}
	gtk_widget_destroy(dialog);
}


void create_window(int argc, char *argv[])
{
	//Init variables
  GtkWidget *main_window;
	SGlobalData data;
	//Init GTK
	gtk_init(&argc, &argv);
	//Build from .glade
  data.builder = gtk_builder_new();
  gtk_builder_add_from_file(data.builder, "GUI.glade", NULL);
	//Get main_window
	main_window =  GTK_WIDGET(gtk_builder_get_object(data.builder,"window_main"));
  parent = main_window;
	//Connect signals
	gtk_builder_connect_signals(data.builder, &data);

  gtk_window_set_title(GTK_WINDOW(main_window), "NitrOCR");
  gtk_widget_show_all(main_window);
  gtk_main();
}

void quitOCR()
{
	gtk_main_quit();
}


int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "GUI.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}
