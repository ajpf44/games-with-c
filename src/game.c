#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "gtk/gtk.h"

static field_slot mat_field[8][8];
int bombs_count = 0;

void reveal_firstclick(field_slot* slot)
{
	slot->is_bomb =false;
  bombs_count = 0;
	init_bombs( slot->x,slot->y);
	output_field();
  
  puts("reveal_firstclick debug");
	printf("value of bombs_count: %d\n", bombs_count);
	printf(" slot->x : %d, slot->y: %d\n", slot->x, slot->y);
	reveal_around(slot);
}

bool check_win(){
   for(int i=0; i < FS; ++i){ 
     for(int j=0; j < FS; ++j){ 
       if( mat_field[i][j].is_bomb 
           && !mat_field[i][j].is_flagged)
         return false;
     }
   }

   return true;
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
		reveal_around(&mat_field[slot->x - 1][slot->y]);
	
	if(slot->x + 1 < FS)
		reveal_around(&mat_field[slot->x + 1][slot->y]);
	
	if(slot->y - 1 >= 0)
		reveal_around(&mat_field[slot->x][slot->y-1]);
	
	if(slot->y + 1 < FS)
		reveal_around(&mat_field[slot->x ][slot->y+1]);
		
	if(slot->y - 1 >= 0 &&slot->x - 1 >= 0)
		reveal_around(&mat_field[slot->x-1][slot->y-1]);

	if(slot->y + 1 < FS &&slot->x - 1 >= 0)
		reveal_around(&mat_field[slot->x-1][slot->y+1]);

	if(slot->y + 1 < FS &&slot->x + 1 < FS)
		reveal_around(&mat_field[slot->x+1][slot->y+1]);

	if(slot->y - 1 >= 0 &&slot->x + 1 < FS)
		reveal_around(&mat_field[slot->x+1][slot->y-1]);
} 

void output_field()
{
	for(int i = 0; i < FS; ++i)
	{
		for(int j = 0; j < FS; ++j)
		{
			if(mat_field[i][j].is_bomb)
				printf("# ");
			else
				printf("%d ", mat_field[i][j].bombs_around);
		}

		puts("");
	}
}

void set_bombs_around()
{
	for(int i = 0; i < FS; ++i)
	{
		for(int j = 0; j < FS; ++j)
		{
			if(mat_field[i][j].is_bomb) 
				continue;
			int bombs_a = 0;
			
			if(i - 1 >=  0)
			{
				bombs_a += mat_field[i-1][j].is_bomb?1:0;
				if(j - 1 >= 0)
					bombs_a += mat_field[i-1][j-1].is_bomb?1:0;
				if(j + 1 < FS)
					bombs_a += mat_field[i-1][j+1].is_bomb?1:0;
			}
			if(i + 1 <  FS)
			{
				bombs_a += mat_field[i+1][j].is_bomb?1:0;
				if(j - 1 >= 0)
					bombs_a += mat_field[i+1][j-1].is_bomb?1:0;
				if(j + 1 < FS)
					bombs_a += mat_field[i+1][j+1].is_bomb?1:0;
			}

			if( j - 1 >= 0)
				bombs_a += mat_field[i][j-1].is_bomb?1:0;

			if( j +1 < FS)
				bombs_a += mat_field[i][j+1].is_bomb?1:0;

			mat_field[i][j].bombs_around = bombs_a;
		}
	}
}

void init_bombs(int i0, int j0)
{
	int par = 1;
	for(int i = 0; i < FS; ++i)
	{
		srand(time(NULL)+i);
		for(int j = 0; j < FS; ++j)
		{
			int rand_par = rand()%2 + par;
			if(
			   i < i0 - rand_par || i > i0+rand_par
			   ||
			   j < j0 - rand_par || j > j0+rand_par
			   )
			{
				int rand_num = rand() % 3;
				if(rand_num==0){
				  ++bombs_count;
				  mat_field[i][j].is_bomb = true;
				}
			}
		}
	}

	set_bombs_around();
}
void reset_slots(field_slot *f)
{
  f->is_bomb = false;
  f->is_revealed = false;
  f->is_flagged = false;
  f->bombs_around = 0;
}

void init_field(field_slot (**field)[FS])
{
	*field = mat_field;
	for(int i = 0; i < FS; ++i)
	{
		for(int j = 0; j < FS; ++j)
		{
      reset_slots(&mat_field[i][j]);

			mat_field[i][j].x = i;
			mat_field[i][j].y = j;
		}
	}
}

int *get_bombscount()
{
  return &bombs_count;
}


