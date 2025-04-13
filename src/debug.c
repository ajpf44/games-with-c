#include <stdio.h>
#include "game.h"
void debug_str(char* str)
{
  printf("reached here in %s\n", str);
}
void debug_slot(field_slot* slot)
{
  printf("debugging slot\n \
  slot->x: %d, slot->y: %d\n \
  slot->bombs_around: %d\n \
  slot->is_bomb: %b\n \
  slot->is_revealed: %b \n\
  slot->is_flagged: %b\n\
finish debug\n",
  slot->x, slot->y, slot->bombs_around,
  slot->is_bomb, slot->is_revealed, slot->is_flagged);
}
void debug_all(field_slot (*field)[FS])
{
  for(int i = 0; i < FS; ++i){
    for(int j = 0; j < FS; ++j){
      debug_slot(&field[i][j]);
    }
  }
}
