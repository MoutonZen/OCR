# include "GUI.h"

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
  SDL_Surface *img = IMG_Load((char *)filename);
  if(img->w > 400 && img->h > 200)
  {
	  printf("Need Resize \n");
	  SDL_Surface *new = Resize(img, 400, 200);
	  SDL_SaveBMP(new,"image_resize");
	  gtk_image_set_from_file (GTK_IMAGE (image), "image_resize");
  }
  else
  {
    gtk_image_set_from_file (GTK_IMAGE (image), filename);
  }
}

//Colors for print
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"


void open_image(GtkButton *button, gpointer user_data) 
{
  struct pair *p = user_data;
  GtkWidget *convert = p->convert;
  GtkWidget *image = p->image;
  GtkWidget *toplevel = gtk_widget_get_toplevel(image);
  GtkFileFilter *filter = gtk_file_filter_new();

  // Open a window to select the image to load
  GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open image",
      GTK_WINDOW (toplevel), GTK_FILE_CHOOSER_ACTION_OPEN,
      "OK", GTK_RESPONSE_ACCEPT, "CANCEL", GTK_RESPONSE_CANCEL,
      NULL);
      
  gtk_file_filter_add_pixbuf_formats(filter);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), filter);

  switch(gtk_dialog_run (GTK_DIALOG (dialog))) {
    case GTK_RESPONSE_ACCEPT:
    {
      filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
      gtk_image_set_from_file(GTK_IMAGE (image), filename);
      gtk_button_set_label(button, "REOPEN IMAGE");

      /* Enable callback to convert functions */
      g_signal_connect(convert, "clicked", G_CALLBACK(run_convert), NULL);
      break;
    }
    default:
    break;
  }
  gtk_widget_destroy(dialog);
}

int launchOCR(GtkButton *button, GtkTextBuffer *buffer)
{
  if(strcmp(filename,"") == 0)
  {
    return 1;
  }
	UNUSED(button);
	SDL_Init(SDL_INIT_VIDEO);
	printf("%s \n ",filename);
	SDL_Surface *img = IMG_Load((char *)filename);
	display_image(img);

	grayscale(img);
	printf("Greyscale \n");
	display_image(img);
    wait_for_keypressed();
	binarize(img);
    printf("Black and white \n");
    display_image(img);
    wait_for_keypressed();
	cutlines(img);
    printf("Line Cuts\n");
    display_image(img);
    wait_for_keypressed();
    int compteur = 0;
    int cl = linenumber(img);
    for(int i = 1; i <= cl; i++){
        SDL_Surface *image_line = displayline(img, i);
        cutchar(image_line);
        int ch = charnumber(image_line);
        for(int j = 1; j < ch; j++){
            SDL_Surface *image_char = displaychar(image_line, j, ch);
            Uint32 pxl;
            Uint8 r, g, b;
            pxl = get_pixel(image_char, 0, 0);
            SDL_GetRGB(pxl, image_char->format, &r, &g, &b);
            char str[12];
            sprintf(str, "%d", compteur+=1);
            char path[14] = "charimage/";
            strcat(path, str);
            SDL_SaveBMP(image_char, path);
			display_image(image_char);
			wait_for_keypressed();
        }
    }
    buffer = buffer;
	SDL_Quit();
	return EXIT_SUCCESS;
}

void trainOCR()
{
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
  gtk_builder_add_from_file(data.builder, "window_main.glade", NULL);
	//Get main_window
	main_window =  GTK_WIDGET(gtk_builder_get_object(data.builder,"window_main"));
  parent = main_window;
	//Connect signals
	gtk_builder_connect_signals(data.builder, &data);

  gtk_window_set_title(GTK_WINDOW(main_window), "Trantor");
  gtk_widget_show_all(main_window);
  gtk_main();
}

void quitOCR()
{
	gtk_main_quit();
}
