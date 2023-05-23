# include <gtk/gtk.h>
# include <stdio.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
} widgets_t;

int main (int argc, char *argv[]) {
    // Inicializa variável builder
    GtkBuilder *builder;
    
    widgets_t *widgets = g_slice_new(widgets_t);
    gtk_init(&argc, &argv);
    
    // Cria builder
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/testgtk.glade", NULL);

    widgets->window = GTK_WIDGET(gtk_builder_get_object (builder, "main-window"));
    widgets->button = GTK_WIDGET(gtk_builder_get_object (builder, "btnShowLabel"));
    widgets->label = GTK_WIDGET(gtk_builder_get_object (builder, "label"));

    gtk_builder_connect_signals(builder, widgets);

    // Apaga o builder
    g_object_unref(builder);

    // Initializa a janela
    gtk_widget_show (widgets->window);
    gtk_main();

    // Apaga o objeto widgets
    g_slice_free (widgets_t, widgets);

    return 0;
}

void on_window_destroy (void) {
    gtk_main_quit();
}

void onclick_btnShowLabel (GtkButton *button, GdkEventKey *event, void *user_data){
    widgets_t *widgets = (widgets_t *)user_data;
    gtk_label_set_text(GTK_LABEL(widgets->label), "Hello World");
}