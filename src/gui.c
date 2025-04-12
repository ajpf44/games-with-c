#include "game.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include <gtk-4.0/gtk/gtk.h>
#include <string.h>

static field_slot (*ptr_field)[8];
static gboolean is_firstclick = true;  
static const char* BOMB_UNI = "\U0001F4A3";
static const char* FLAG_UNI = "\U00002691";
static  GtkWidget *tview;
static int flags_count = 0;
static void stop_gui();
typedef enum{
  WINNER,
  LOSER,
  GAME
} game_val_t;

static void update_info(game_val_t gval)

{
  char str[32];
  
  GtkTextBuffer *buf_view = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tview));;
  int *bcount_ptr= get_bombscount();
  
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
  }

  gtk_text_buffer_set_text(buf_view, str, -1);
} 

static void reveal_slot (GtkWidget *button, field_slot* slot)
{
  g_print("click x = %d, y = %d\n", slot->x, slot->y);

  if(is_firstclick)	{
    reveal_firstclick(slot);
    update_info(false);
    is_firstclick = false;
    return;
  }
  if(slot->is_revealed) 
    return;

  if(slot->is_bomb)	{
    gtk_button_set_label(GTK_BUTTON(button), BOMB_UNI);
    update_info(LOSER);
    stop_gui(); 
  }
  else if(slot->bombs_around == 0)	{
    reveal_around(slot);
  }
  else	{
    char s[2];
    sprintf(s, "%d", slot->bombs_around);
    gtk_button_set_label(GTK_BUTTON(button), s);
  }

  slot->is_revealed = true;
}



static void put_flag (field_slot* slot){
  if(slot->is_revealed)
    return;

  if(slot->is_flagged){
    flags_count--;
    gtk_button_set_label(GTK_BUTTON(slot->button), " ");
  } else{
    flags_count++;
    gtk_button_set_label(GTK_BUTTON(slot->button), FLAG_UNI);
  }

  if(*get_bombscount() == flags_count && check_win()){
    stop_gui();
    update_info(WINNER);
  }else{
    update_info(false);
  }

  slot->is_flagged = !slot->is_flagged;
}

static void stop_gui()
{
  for(int i = 0; i < FS; ++i){
    for(int j = 0; j < FS; ++j){
      g_signal_handlers_disconnect_by_func( 
          ptr_field[i][j].button,
          G_CALLBACK(reveal_slot),
          &ptr_field[i][j]
          );
      g_signal_handlers_disconnect_by_func( 
          ptr_field[i][j].gest,
          G_CALLBACK(put_flag),
          &ptr_field[i][j]
          );
    }
  } 
}

static void activate (GtkApplication *app, field_slot field[][FS])
{
  GtkWidget *window, *box,*box_info;
  GtkTextBuffer *buf;
  int info_mrg = 10;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Campo minado, de ajpf44");
  gtk_window_present (GTK_WINDOW(window));

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_valign (box, GTK_ALIGN_CENTER);
  gtk_window_set_child(GTK_WINDOW(window), box);	

  box_info = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_valign (box_info, GTK_ALIGN_CENTER);	    
  gtk_widget_set_margin_top(box_info, info_mrg);
  gtk_widget_set_margin_bottom(box_info, info_mrg);
  gtk_widget_set_margin_start(box_info, info_mrg);
  gtk_box_append(GTK_BOX(box), box_info);

  tview = gtk_text_view_new();
  buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tview));
  gtk_text_buffer_set_text(buf, "Start the Game\n", -1);
  gtk_widget_set_hexpand(tview, true);
  gtk_box_append(GTK_BOX(box_info), tview);

  for( int i = 0; i < FS; ++i){
    GtkWidget *box_line;
    box_line = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_box_append(GTK_BOX(box), box_line);
    for( int j = 0; j < FS; ++j){
      GtkWidget* button;
      button =  gtk_button_new_with_label(" ");
      field[i][j].button = button;
      g_signal_connect(button, "clicked", G_CALLBACK(reveal_slot), &field[i][j]);

      GtkGesture *gest;
      gest = gtk_gesture_click_new();
      gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gest), 0);
      field[i][j].gest = gest;

      //g_signal_connect(gest, "clicked", G_CALLBACK(put_flag), &field[i][j]);
      //verify if its posisble change from g_signal_connect_data to g_signal_connect
      // g_signal_connect_swapped(gest, "clicked", G_CALLBACK(put_flag), &field[i][j]);
      g_signal_connect_data (
          gest,
          "released",
          G_CALLBACK(put_flag),
          &field[i][j],
          NULL,
          G_CONNECT_SWAPPED
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
  ptr_field = field;
  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), field);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
