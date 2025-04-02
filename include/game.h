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
} field_slot;

void output_field(field_slot field[][FS]);
void init_bombs(field_slot field[][FS], int init_i, int init_j);
void reveal_around(field_slot *slot, field_slot field[][FS]);
void reveal_firstclick(field_slot* slot, field_slot (*ptr_f)[8]);
void init_field(field_slot (*field)[FS]);


