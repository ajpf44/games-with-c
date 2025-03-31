#include "game.h"
#include "gui.h"

static field_slot mat_field[8][8];

int main(int argc, char** argv)
{	
	
	
	init_bombs(mat_field);
	output_field(mat_field);
	int status = gtk_main(argc, argv, mat_field);
    return 0;
}
