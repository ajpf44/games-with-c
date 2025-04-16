#include "game.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include <gtk-4.0/gtk/gtk.h>
#include "debug.h"
// stores all data from each slot of the game
static field_slot (*f)[8];

static gboolean is_firstclick = true;  

// unicodes for the bombs and flag
static char* BOMB_UNI = "\U0001F4A3";
static char* FLAG_UNI = "\U00002691";

// need to get back in direcoty because usually it builds from dir /build
static const char* css_path = "../src/gui.css";

// text view used to display info on the screen
// how many bombs are, how many flags and if you have lose or win
static GtkWidget *tview;

static int flags_count = 0;

static void stop_game();

// used in switch in update_info
typedef enum
{
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

static void reveal_slot(field_slot *slot, const char* str)
{
    slot->is_revealed = true;
    gtk_button_set_label(GTK_BUTTON(slot->button), str);

    if(slot->is_bomb) {
        gtk_widget_add_css_class(slot->button, "bomb");
        return;
    }
        
    char css_class[16];
    sprintf(css_class, "bombs_around_%d", slot->bombs_around);
    gtk_widget_add_css_class(slot->button, css_class);
}

void reveal_around(field_slot* slot)
{
    debug_slot(slot);
    if(slot->is_revealed || slot->is_bomb)
        return;

    char bombsaround_str[2];
    sprintf(bombsaround_str, "%d", slot->bombs_around);
    reveal_slot(slot, bombsaround_str);

    if(slot->bombs_around != 0)
        return;

    for(int i=-1 ; i < 2; ++i){
        for(int j=-1 ; j < 2; ++j){
            if(slot->x + i >= 0 && slot->x + i < FS && slot->y + j >= 0 && slot->y + j < FS)
                reveal_around(&f[slot->x +i][slot->y + j]);
        }
    }
}

void reveal_firstclick(field_slot* slot)
{
    slot->is_bomb =false;
    game_init_bombs( slot->x,slot->y);
    game_output_field();

    debug_str("aqui");
    reveal_around(slot);
    debug_str("chegou aqui");
}

static void handles_reveal (GtkWidget *button, field_slot* slot)
{
    if(is_firstclick){
        reveal_firstclick(slot);
        update_info(GAME);
        is_firstclick = false;
        return;
    }

    if(slot->is_revealed) 
        return;

    if(slot->is_bomb){
        reveal_slot(slot, BOMB_UNI);
        update_info(LOSER);
        stop_game(); 
    } else if(slot->bombs_around == 0){
        reveal_around(slot);
    } else{
        char s[2];
        sprintf(s, "%d", slot->bombs_around);
        gtk_button_set_label(GTK_BUTTON(button), s);
        reveal_slot(slot, s);
    }
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

            gtk_widget_set_css_classes(f[i][j].button, NULL);
            gtk_widget_add_css_class(f[i][j].button, "btn_slot");

            g_signal_connect(f[i][j].button,"clicked",G_CALLBACK(handles_reveal),&f[i][j]);
            g_signal_connect_swapped(f[i][j].gest,"released", G_CALLBACK(put_flag),&f[i][j]);
        }
    }

}

static void stop_game()
{
    for(int i = 0; i < FS; ++i){
        for(int j = 0; j < FS; ++j){
            g_signal_handlers_disconnect_by_func(f[i][j].button, G_CALLBACK(handles_reveal) ,&f[i][j]);
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
            GtkWidget *button;
            GtkGesture *gest;

            gtk_widget_add_css_class(button, "btn-slot");

            button =  gtk_button_new_with_label(" ");
            f[i][j].button = button;

            gest = gtk_gesture_click_new();
            gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gest), 3);
            f[i][j].gest = gest;

            g_signal_connect(button,"clicked", G_CALLBACK(handles_reveal), &f[i][j]);
            g_signal_connect_swapped(gest, "released", G_CALLBACK(put_flag), &f[i][j]);

            gtk_widget_add_controller(GTK_WIDGET(button), GTK_EVENT_CONTROLLER(gest));

            gtk_widget_set_css_classes(button, NULL);
            gtk_widget_add_css_class(button, "btn_slot");

            gtk_box_append(GTK_BOX(line_container), button);
        }
    }
}
static void gui_set_cssprovider()
{
    GtkCssProvider* css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(css_provider), css_path);
    GdkDisplay* display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(
            display,
            GTK_STYLE_PROVIDER(css_provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
            );
}

static void debug_func(){
    puts("\n### pritnign field");
    game_output_field();
    puts("############################\n");
}

static void activate (GtkApplication *app, field_slot field[][FS])
{
    gui_set_cssprovider();
    GtkWidget *window, *box_btns,*box_info, *btn_restart;
    GtkTextBuffer *buf;
    int info_mrg = 10;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ajpf44's minesweeper");
    gtk_window_present (GTK_WINDOW(window));
    gtk_window_set_default_size(GTK_WINDOW(window), 400,400);

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
    gtk_widget_add_css_class(tview, "tview");

    //btn to restart the game
    btn_restart = gtk_button_new_with_label("Restart");
    g_signal_connect(btn_restart, "clicked", G_CALLBACK(restart_game),NULL);
    gtk_box_append(GTK_BOX(box_info), btn_restart);
    
    //btn to restart the game
    GtkWidget* btn_debug;
    btn_debug = gtk_button_new_with_label("debug");
    g_signal_connect(btn_debug, "clicked", G_CALLBACK(debug_func),NULL);
    gtk_box_append(GTK_BOX(box_info), btn_debug);

    // add each slot as a button on box_btns
    append_slots(box_btns);
    gtk_widget_add_css_class(box_btns, "box");
//
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
