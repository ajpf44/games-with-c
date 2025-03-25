#include <stdio.h>
#include <stdbool.h>

#include "move.h"
#include "pieces.h"
#include "output.h"


//PLAYER_1 WHITE
//PLAYER_2 BLACK'
typedef enum{
	PLAYER_1,
	PLAYER_2
} player_round_t;

int main (int argc, char** argv)
{
    void set_board_init_pices(int board[8][8]);

	puts("ajpf's chess, version 0.1");

	int board[8][8];

	set_board_init_pices(board);

	bool is_player1_round = true;

    while(1)
    {    
        output_board(board);
        
        pos piece;
        pos desired;
        
        printf("%s piece: ", is_player1_round?"p1":"p2");
        scanf("%d-%d", &piece.x, &piece.y);
        printf("%s desired: ", is_player1_round?"p1":"p2");
        scanf("%d-%d", &desired.x, &desired.y);
		
        move_result_t move_result = 
            analize_move(piece, desired, board, board[piece.x][piece.y], is_player1_round?W_CODE:B_CODE);

        if(move_result == CAN_MOVE)
        {
            puts("moving");
            move_piece(piece, desired, board, board[piece.x][piece.y]);;
            is_player1_round = !is_player1_round;
        }
        else if(move_result == CAN_CAPTURE)
        {
            puts("capturing a piece");
            move_piece(piece, desired, board, board[piece.x][piece.y]);
        }
        else
            puts("cannot move");
        puts("##################################");
    }

    output_board(board);
	return 0;
}

//WIP
void get_piece_to_play(pos* des, player_round_t player, int board[8][8])
{
	pos in;
	printf("%s piece: ", player==PLAYER_1?"p1":"p2");
    scanf("%d-%d", &in.x, &in.y);

    if ( player == PLAYER_1 && board[in.x][in.y] < W_PAWN)
    {
    	// printf("")
    }
}

void set_board_init_pices(int board[8][8])
{
    for(int i =0; i < 8; ++i)
	{
		for(int j =0; j < 8; ++j)
		{
			if (i ==0 || i == 7)
			{
				switch (j) 
				{
					case 0:case 7:
						board[i][j] =  i==0?B_TOWER:W_TOWER;
						break;
					case 1:case 6:
						board[i][j] = i==0?B_HORSE:W_HORSE;
						break;
					case 2:	case 5:
						board[i][j] = i==0?B_BISHOP:W_BISHOP;
						break;
					case 3:
						board[i][j] = i==0?B_KING:W_KING;
						break;
					case 4:
						board[i][j] = i==0?B_QUEEN:W_QUEEN;
						break;
				}
			}else if(i == 1 || i == 6)
			{
				board[i][j] = i==1?B_PAWN:W_PAWN;
			}
			else
			{
				board[i][j] = NO_PIECE;
			}
		}
	}
}

