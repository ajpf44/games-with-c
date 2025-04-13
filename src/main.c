#include "game.h"
#include "gui.h"

int main(int argc, char** argv)
{
  field_slot (*mat_field)[FS] = malloc(8 * sizeof(field_slot[8]));
  
  game_init_field(&mat_field);
  
  int status = gtk_main(argc, argv, mat_field);
  return status;
}
