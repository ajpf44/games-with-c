#include "game.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include <gtk-4.0/gtk/gtk.h>

static gboolean is_firstclick = true;  

static void reveal_slot (GtkWidget *button, field_slot* slot)
{
	g_print("click x = %d, y = %d\n", slot->x, slot->y);

	if(is_firstclick)
	{
		reveal_firstclick(slot);
		is_firstclick = false;
		return;
	}
		
	if(slot->is_revealed) return;
	
	if(slot->is_bomb)
	{
		gtk_button_set_label(GTK_BUTTON(button), "#");
	}
	else if(slot->bombs_around == 0)
	{
		reveal_around(slot);
	}
	else
	{
		char s[2];
		sprintf(s, "%d", slot->bombs_around);
		gtk_button_set_label(GTK_BUTTON(button), s);
	}

	slot->is_revealed = true;
}

static void w_bombflag (GtkWidget *button, field_slot* slot)
{
	 gtk_button_set_label(GTK_BUTTON(button), "?");
}

static void activate (GtkApplication *app, field_slot field[][FS])
{
	GtkWidget* window;
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Campo minado, de ajpf44");
	gtk_window_present (GTK_WINDOW(window));
	
	GtkWidget *box;
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
	gtk_widget_set_valign (box, GTK_ALIGN_CENTER);

	gtk_window_set_child(GTK_WINDOW(window), box);	
	
	for( int i = 0; i < FS; ++i)
	{
		GtkWidget *box_line;
		box_line = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
		
		gtk_box_append(GTK_BOX(box), box_line);
		for( int j = 0; j < FS; ++j)
		{
			GtkWidget* button;
			button =  gtk_button_new_with_label(" ");
			field[i][j].button = button;
			g_signal_connect(button, "clicked", G_CALLBACK(reveal_slot), &field[i][j]);

			GtkGesture *gest;
			gest = gtk_gesture_click_new();
			gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gest), 0);

			// g_signal_connect(gest, "released",G_CALLBACK(w_bombflag) , &field[i][j]);
			g_signal_connect_object(
				gest,
				"released",
				G_CALLBACK(w_bombflag),
				button, G_CONNECT_SWAPPED
				);
			gtk_widget_add_controller(GTK_WIDGET(button), GTK_EVENT_CONTROLLER(gest));
			
			gtk_box_append(GTK_BOX(box_line), button);
		}
	}
}

int gtk_main(int argc, char** argv, field_slot field[][FS])
{
	GtkApplication *app;
	int status;	
		
	app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), field);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
		
	return status;
}
