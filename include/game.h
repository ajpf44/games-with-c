#pragma once
#include "gtk/gtk.h"
#include <stdbool.h>

static const int FS = 8;

typedef struct 
{
	GtkWidget* button;
	int bombs_around;
	int x;
	int y;
	bool is_bomb;
	bool is_revealed;
	bool is_flagged;
} field_slot;

void output_field();
void init_bombs(int init_i, int init_j);
void reveal_around(field_slot *slot);
void reveal_firstclick(field_slot* slot);
void init_field(field_slot (**field)[FS]);
int *get_bombscount();
bool check_win();
