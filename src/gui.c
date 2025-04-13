#include "game.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include <gtk-4.0/gtk/gtk.h>

// stores all data from each slot of the game
static field_slot (*f)[8];

static gboolean is_firstclick = true;  

// unicodes for the bombs and flag
static const char* BOMB_UNI = "\U0001F4A3";
static const char* FLAG_UNI = "\U00002691";

// text view used to display info on the screen
// how many bombs are, how many flags and if you have lose or win
static GtkWidget *tview;

static int flags_count = 0;

static void stop_game();

// used in switch in update_info
typedef enum{
    WINNER,
    LOSER,
    GAME,
    RESTART
} game_val_t;

static void update_info(game_val_t gval)
{
    char str[32];
    int *bcount_ptr= game_get_bombscount();
    GtkTextBuffer *buf_view = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tview));;

    switch(gval){
        case WINNER:
            sprintf(str, "YOU WIN\nCongratulations!!!");
            break;
        case LOSER:
            sprintf(str, "YOU LOSE\n looooooseeeeer!!");
            break;
        case GAME:
            sprintf(str, "INFO: [%s: %d]  -  [%s: %d]\n", BOMB_UNI, *bcount_ptr, FLAG_UNI, flags_count);
            break;
        case RESTART:
            sprintf(str, "GAME RESTARTED\nyou can play");
            break;
    }

    gtk_text_buffer_set_text(buf_view, str, -1);
} 

void reveal_around(field_slot* slot)
{
  if(slot->is_revealed || slot->is_bomb)
    return;

  slot->is_revealed = true;
	char buf[2];
	sprintf(buf, "%d", slot->bombs_around);
	gtk_button_set_label(GTK_BUTTON(slot->button), buf);
	
	if(slot->bombs_around != 0)
		return;

	if(slot->x - 1 >= 0)
		reveal_around(&f[slot->x - 1][slot->y]);
	
	if(slot->x + 1 < FS)
		reveal_around(&f[slot->x + 1][slot->y]);
	
	if(slot->y - 1 >= 0)
		reveal_around(&f[slot->x][slot->y-1]);
	
	if(slot->y + 1 < FS)
		reveal_around(&f[slot->x ][slot->y+1]);
		
	if(slot->y - 1 >= 0 &&slot->x - 1 >= 0)
		reveal_around(&f[slot->x-1][slot->y-1]);

	if(slot->y + 1 < FS &&slot->x - 1 >= 0)
		reveal_around(&f[slot->x-1][slot->y+1]);

	if(slot->y + 1 < FS &&slot->x + 1 < FS)
		reveal_around(&f[slot->x+1][slot->y+1]);

	if(slot->y - 1 >= 0 &&slot->x + 1 < FS)
		reveal_around(&f[slot->x+1][slot->y-1]);
}

void reveal_firstclick(field_slot* slot)
{
    slot->is_bomb =false;
    game_init_bombs( slot->x,slot->y);
    game_output_field();

    reveal_around(slot);
}

static void reveal_slot (GtkWidget *button, field_slot* slot)
{
    if(is_firstclick)	{
        reveal_firstclick(slot);
        update_info(GAME);
        is_firstclick = false;
        return;
    }

    if(slot->is_revealed) 
        return;

    if(slot->is_bomb)	{
        gtk_button_set_label(GTK_BUTTON(button), BOMB_UNI);
        update_info(LOSER);
        stop_game(); 
    } else if(slot->bombs_around == 0)	{
        reveal_around(slot);
    } else	{
        char s[2];
        sprintf(s, "%d", slot->bombs_around);
        gtk_button_set_label(GTK_BUTTON(button), s);
    }

    slot->is_revealed = true;
}

static void put_flag (field_slot* slot)
{
    if(slot->is_revealed)
        return;

    if(slot->is_flagged){
        flags_count--;
        gtk_button_set_label(GTK_BUTTON(slot->button), " ");
        slot->is_flagged = false;
    } else{
        flags_count++;
        gtk_button_set_label(GTK_BUTTON(slot->button), FLAG_UNI);
        slot->is_flagged = true;
    }

    if(*game_get_bombscount() == flags_count && !is_firstclick && game_check_win()){
        stop_game();
        update_info(WINNER);
    }else{
        update_info(GAME);
    }

}
void restart_game()
{
    flags_count = 0;
    is_firstclick = true;
    *game_get_bombscount() = 0;

    stop_game();
    update_info(RESTART);

    for(int i = 0; i < FS; ++i){
        for(int j = 0; j < FS; ++j){
            gtk_button_set_label(GTK_BUTTON(f[i][j].button), " ");
            game_reset_slots(&f[i][j]);

            g_signal_connect(f[i][j].button,"clicked",G_CALLBACK(reveal_slot),&f[i][j]);
            g_signal_connect_swapped(f[i][j].gest,"released", G_CALLBACK(put_flag),&f[i][j]);
        }
    }

}

static void stop_game()
{
    for(int i = 0; i < FS; ++i){
        for(int j = 0; j < FS; ++j){
            g_signal_handlers_disconnect_by_func(f[i][j].button, G_CALLBACK(reveal_slot) ,&f[i][j]);

            g_signal_handlers_disconnect_by_func(f[i][j].gest, G_CALLBACK(put_flag), &f[i][j]);
        }
    } 
}

static void append_slots(GtkWidget* box_btns)
{
    for( int i = 0; i < FS; ++i){
        GtkWidget *line_container;
        line_container = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_box_append(GTK_BOX(box_btns), line_container);

        for( int j = 0; j < FS; ++j){
            GtkWidget* button;
            GtkGesture *gest;

            button =  gtk_button_new_with_label(" ");
            f[i][j].button = button;

            gest = gtk_gesture_click_new();
            gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gest), 3);
            f[i][j].gest = gest;

            g_signal_connect(button,"clicked", G_CALLBACK(reveal_slot), &f[i][j]);

            g_signal_connect_swapped(gest, "released", G_CALLBACK(put_flag), &f[i][j]);

            gtk_widget_add_controller(GTK_WIDGET(button), GTK_EVENT_CONTROLLER(gest));

            gtk_box_append(GTK_BOX(line_container), button);
        }
    }
}

static void activate (GtkApplication *app, field_slot field[][FS])
{
    GtkWidget *window, *box_btns,*box_info, *btn_restart;
    GtkTextBuffer *buf;
    int info_mrg = 10;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ajpf44's minesweeper");
    gtk_window_present (GTK_WINDOW(window));
    
    // game container, that contain the buttons
    box_btns = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_valign (box_btns, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), box_btns);	

    // container for information
    box_info = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_valign (box_info, GTK_ALIGN_CENTER);	    
    gtk_widget_set_margin_top(box_info, info_mrg);
    gtk_widget_set_margin_bottom(box_info, info_mrg);
    gtk_widget_set_margin_start(box_info, info_mrg);
    gtk_box_append(GTK_BOX(box_btns), box_info);

    // render the text on box_info
    tview = gtk_text_view_new();
    buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tview));
    gtk_text_buffer_set_text(buf, "Start the Game\n", -1);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (tview), false);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (tview), false);
    gtk_widget_set_hexpand(tview, true);
    gtk_box_append(GTK_BOX(box_info), tview);
    
    //btn to restart the game
    btn_restart = gtk_button_new_with_label("Restart");
    g_signal_connect(btn_restart, "clicked", G_CALLBACK(restart_game),NULL);
    gtk_box_append(GTK_BOX(box_info), btn_restart);

    // add each slot as a button on box_btns
    append_slots(box_btns);
}

int gtk_main(int argc, char** argv, field_slot field[][FS])
{
    GtkApplication *app;
    int status;	
    
    f = field;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), field);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
