#include "game.h"
#include "gui.h"

int main(int argc, char** argv)
{
  field_slot (*field)[FS];
  init_field(field);
	int status = gtk_main(argc, argv, mat_field);
  return status;
}
