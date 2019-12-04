#include "GUI.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "GUI.glade", NULL);

    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    
    // get pointers to the two labels
    GTK_WIDGET(gtk_builder_get_object(builder, "lbl_hello"));
    GTK_WIDGET(gtk_builder_get_object(builder, "lbl_count"));

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}