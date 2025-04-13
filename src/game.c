#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

static field_slot mat_field[8][8];
int bombs_count = 0;

bool game_check_win()
{
    for(int i=0; i < FS; ++i){ 
        for(int j=0; j < FS; ++j){ 
            if( mat_field[i][j].is_bomb && !mat_field[i][j].is_flagged)
                return false;
        }
    }

    return true;
}

void game_output_field()
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

void game_init_bombs(int i0, int j0)
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
void game_reset_slots(field_slot *f)
{
    f->is_bomb = false;
    f->is_revealed = false;
    f->is_flagged = false;
    f->bombs_around = 0;
}

void game_init_field(field_slot (**field)[FS])
{
    *field = mat_field;
    for(int i = 0; i < FS; ++i)
    {
        for(int j = 0; j < FS; ++j)
        {
            game_reset_slots(&mat_field[i][j]);

            mat_field[i][j].x = i;
            mat_field[i][j].y = j;
        }
    }
}

int *game_get_bombscount()
{
    return &bombs_count;
}


