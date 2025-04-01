#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "gtk/gtk.h"
void reveal_around(field_slot* slot, field_slot (*ptr_f)[8]){
	if(
		slot->bombs_around == 0 &&
		 slot->is_revealed == false)
	{	
		slot->is_revealed = true;
		char s[2];
		sprintf(s, "%d", slot->bombs_around);
		gtk_button_set_label(GTK_BUTTON(slot->button), s);
		puts("teste");
	}
	else if(slot->is_bomb == false)
	{
		// slot->is_revealed = true;
		char s[2];
		sprintf(s, "%d", slot->bombs_around);
		gtk_button_set_label(GTK_BUTTON(slot->button), s);
		return;
	}

	if(slot->x - 1 >= 0)
		reveal_around(&ptr_f[slot->x - 1][slot->y],ptr_f);
	
	if(slot->x + 1 < FS)
		reveal_around(&ptr_f[slot->x + 1][slot->y],ptr_f);
	
	if(slot->y - 1 >= 0)
		reveal_around(&ptr_f[slot->x][slot->y-1],ptr_f);
	
	if(slot->y + 1 < FS)
		reveal_around(&ptr_f[slot->x ][slot->y+1],ptr_f);
} 

void output_field(field_slot field[][FS])
{
	for(int i = 0; i < FS; ++i)
	{
		for(int j = 0; j < FS; ++j)
		{
			if(field[i][j].is_bomb)
				printf("# ");
			else
				printf("%d ", field[i][j].bombs_around);
		}

		puts("");
	}
}

void set_bombs_around(field_slot field[][FS])
{
	for(int i = 0; i < FS; ++i)
	{
		for(int j = 0; j < FS; ++j)
		{
			if(field[i][j].is_bomb) continue;
			int bombs_a = 0;
			
			if(i - 1 >=  0)
			{
				bombs_a += field[i-1][j].is_bomb?1:0;
				if(j - 1 >= 0)
					bombs_a += field[i-1][j-1].is_bomb?1:0;
				if(j + 1 < FS)
					bombs_a += field[i-1][j+1].is_bomb?1:0;
			}
			if(i + 1 <  FS)
			{
				bombs_a += field[i+1][j].is_bomb?1:0;
				if(j - 1 >= 0)
					bombs_a += field[i+1][j-1].is_bomb?1:0;
				if(j + 1 < FS)
					bombs_a += field[i+1][j+1].is_bomb?1:0;
			}

			if( j - 1 >= 0)
				bombs_a += field[i][j-1].is_bomb?1:0;

			if( j +1 < FS)
				bombs_a += field[i][j+1].is_bomb?1:0;

			field[i][j].bombs_around = bombs_a;
		}
	}
}

void init_bombs(field_slot field[][FS])
{
	for(int i = 0; i < FS; ++i)
	{
		srand(time(NULL)+i);
		for(int j = 0; j < FS; ++j)
		{
			field[i][j].is_bomb= false;
			field[i][j].is_revealed= false;
			field[i][j].x = i;
			field[i][j].y = j;
						
			int rand_num = rand() % 5;
			if(rand_num==0)
				field[i][j].is_bomb = true;			
		}
	}

	set_bombs_around(field);
}
