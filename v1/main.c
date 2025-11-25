#include <gtk/gtk.h>
#include <string.h>
#include "file.c"

typedef struct {
    GtkWidget* window;
    GtkTextBuffer* buffer;
}OpenData;

typedef struct {
    GtkWidget* window;
    GtkTextBuffer* buffer;
}SaveData;

typedef struct {
    GtkWidget* window;
    GtkTextBuffer* buffer;
}SaveAsData;

char save_path[1028];

void on_new_mi_clicked(GtkWidget* widget, gpointer data) {
    GtkTextBuffer* buffer = data;

    gtk_text_buffer_set_text(buffer, "", -1);
    strcpy(save_path, "");
}

void on_open_mi_clicked(GtkWidget* widget, gpointer data) {
    OpenData* open_data = (OpenData*)data;
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(open_data->buffer);

    GtkWidget* dialog;
    dialog = gtk_file_chooser_dialog_new(
        "Open File",
        GTK_WINDOW(open_data->window),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Open", GTK_RESPONSE_ACCEPT,
        NULL
    );

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        char* text = readf(filename);
        if (!text) {
            dialog = gtk_message_dialog_new(
                GTK_WINDOW(open_data->window),
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "파일을 열 수 없음."
            );
            gtk_dialog_run(GTK_DIALOG(dialog));
        }
        else {
            strcpy(save_path, filename);
            gtk_text_buffer_set_text(buffer, text, -1);
            g_free(filename);
            free(text);
            text = NULL;
        }
    }

    gtk_widget_destroy(dialog);
}

void on_save_mi_clicked(GtkWidget* widget, gpointer data) {
    SaveData* save_data = (SaveData*)data;
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(save_data->buffer);

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);

    gchar* text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    if (strcmp(save_path, "")) {
        writef(save_path, text);
        g_free(text);
        return;
    }

    GtkWidget* dialog;
    dialog = gtk_file_chooser_dialog_new(
        "Save File",
        GTK_WINDOW(save_data->window),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Save", GTK_RESPONSE_ACCEPT,
        NULL
    );

    GtkFileFilter* text_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(text_filter, "Text Files(*.txt)");
    gtk_file_filter_add_pattern(text_filter, "*.txt");

    GtkFileFilter* md_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(md_filter, "Markdown Files(*.md)");
    gtk_file_filter_add_pattern(md_filter, "*.md");

    GtkFileFilter* all_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(all_filter, "All Files(*)");
    gtk_file_filter_add_pattern(all_filter, "*");

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), text_filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), md_filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), all_filter);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        if (writef(filename, text)) {
            dialog = gtk_message_dialog_new(
                GTK_WINDOW(save_data->window),
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "파일을 저장할 수 없음."
            );
            gtk_dialog_run(GTK_DIALOG(dialog));
        }
        else {
            strcpy(save_path, filename);
            g_free(filename);
            g_free(text);
        }
    }

    gtk_widget_destroy(dialog);
}

void on_save_as_mi_clicked(GtkWidget* widget, gpointer data) {
    SaveData* save_as_data = (SaveData*)data;
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(save_as_data->buffer);

    GtkWidget* dialog;
    dialog = gtk_file_chooser_dialog_new(
        "Save As File",
        GTK_WINDOW(save_as_data->window),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Save", GTK_RESPONSE_ACCEPT,
        NULL
    );

    GtkFileFilter* text_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(text_filter, "Text Files(*.txt)");
    gtk_file_filter_add_pattern(text_filter, "*.txt");

    GtkFileFilter* md_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(md_filter, "Markdown Files(*.md)");
    gtk_file_filter_add_pattern(md_filter, "*.md");

    GtkFileFilter* all_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(all_filter, "All Files(*)");
    gtk_file_filter_add_pattern(all_filter, "*");

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), text_filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), md_filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), all_filter);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);

        gchar* text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        if (writef(filename, text)) {
            dialog = gtk_message_dialog_new(
                GTK_WINDOW(save_as_data->window),
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "파일을 저장할 수 없음."
            );
            gtk_dialog_run(GTK_DIALOG(dialog));
        }
        else {
            strcpy(save_path, filename);
            g_free(filename);
            g_free(text);
        }
    }

    gtk_widget_destroy(dialog);
}

void on_exit_mi_clicked(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);
    
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "np++");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkAccelGroup* accel = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel);

    GtkWidget* textview = gtk_text_view_new();
    GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), textview);

    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

    GtkWidget* menubar = gtk_menu_bar_new();

    GtkWidget* sep = gtk_separator_menu_item_new(); // 구분선

    GtkWidget* file_menu = gtk_menu_new();
    GtkWidget* file_mi = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_mi), file_menu);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_mi);

    GtkWidget* new_mi = gtk_menu_item_new_with_label("New");
    GtkWidget* open_mi = gtk_menu_item_new_with_label("Open");
    GtkWidget* save_mi = gtk_menu_item_new_with_label("Save");
    GtkWidget* save_as_mi = gtk_menu_item_new_with_label("Save As");
    GtkWidget* exit_mi = gtk_menu_item_new_with_label("Exit");

    OpenData* open_data = g_malloc(sizeof(OpenData*));
    open_data->window = window;
    open_data->buffer = buffer;

    SaveData* save_data = g_malloc(sizeof(SaveData*));
    save_data->window = window;
    save_data->buffer = buffer;

    SaveAsData* save_as_data = g_malloc(sizeof(SaveAsData*));
    save_as_data->window = window;
    save_as_data->buffer = buffer;

    g_signal_connect(new_mi, "activate", G_CALLBACK(on_new_mi_clicked), buffer);
    g_signal_connect(open_mi, "activate", G_CALLBACK(on_open_mi_clicked), open_data);
    g_signal_connect(save_mi, "activate", G_CALLBACK(on_save_mi_clicked), save_data);
    g_signal_connect(save_as_mi, "activate", G_CALLBACK(on_save_as_mi_clicked), save_as_data);
    g_signal_connect(exit_mi, "activate", G_CALLBACK(on_exit_mi_clicked), NULL);

    gtk_widget_add_accelerator(new_mi, "activate", accel, GDK_KEY_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(open_mi, "activate", accel, GDK_KEY_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(save_mi, "activate", accel, GDK_KEY_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(save_as_mi, "activate", accel, GDK_KEY_s, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(exit_mi, "activate", accel, GDK_KEY_e, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), new_mi);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_mi);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_mi);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_as_mi);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit_mi);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}