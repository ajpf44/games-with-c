#pragma once
#include "gtk/gtk.h"
#include <stdbool.h>

static const int FS = 8;

typedef struct 
{
	GtkWidget* button;
  GtkGesture *gest;
	int bombs_around;
	int x;
	int y;
	bool is_bomb;
	bool is_revealed;
	bool is_flagged;
} field_slot;

void game_output_field();
void game_init_bombs(int init_i, int init_j);
void game_reveal_firstclick(field_slot* slot);
void game_init_field(field_slot (**field)[FS]);
void game_reset_slots(field_slot *f);
int *game_get_bombscount();
bool game_check_win();
