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
    pos scan_piece(player_round_t player, int board[8][8]);
    pos scan_desired(player_round_t player, int board[8][8]);

	puts("ajpf's chess, version 0.1");

	int board[8][8];

	set_board_init_pices(board);

	player_round_t player = PLAYER_1;

    while(1)
    {    
		output_board(board);
		
		pos piece = scan_piece(player, board);
		pos desired = scan_desired(player, board);
		
		move_result_t move_result = 
			analize_move(piece, desired, board, board[piece.x][piece.y], player==PLAYER_1?W_CODE:B_CODE);

		if(move_result == CAN_MOVE)
		{
			puts("moving");
			move_piece(piece, desired, board, board[piece.x][piece.y]);;
			player = player==PLAYER_1?PLAYER_2:PLAYER_1;
		}
		else if(move_result == CAN_CAPTURE)
		{
			puts("capturing a piece");

			if(board[desired.x][desired.y] == W_KING || board[desired.x][desired.y] == B_KING)
			{
				char* winner = (player==PLAYER_1)?"Player_1":"Player_2";
				printf("Victory from player %s", winner);
				break;
			}
			
			move_piece(piece, desired, board, board[piece.x][piece.y]);
			player = player==PLAYER_1?PLAYER_2:PLAYER_1;
		}
		else
			puts("cannot move\n");
			
		puts("##################################");
    }

    output_board(board);
	return 0;
}

//WIP

bool is_invalid_piece(pos input, player_round_t player, int board[8][8], char** s)
{
	if(input.x > 7 || input.y> 7  ||  input.x < 0 || input.y < 0)
	{
		*s = "outbounds the board";
		return true;
	}

	if( board[input.x][input.y] == NO_PIECE)
	{
		*s = "there is no piece to move";
		return true;
	}
	
    if ( 
    	(player == PLAYER_1 && board[input.x][input.y] < W_PAWN) ||
    	(player == PLAYER_2 && board[input.x][input.y] >= W_PAWN)
    )
    {
		*s = "you cannot move that piece";
		return true;
    }

    return false;
}

pos scan_desired(player_round_t player, int board[8][8])
{
	pos in;
	printf("%s desired: ", player==PLAYER_1?"p1":"p2");
    scanf("%d-%d", &in.x, &in.y);

    if(in.x > 7 || in.y> 7  ||  in.x < 0 || in.y < 0)
	{
		puts("outbounds the board");
		return scan_desired(player, board);
	}

	return in;
}

pos scan_piece(player_round_t player, int board[8][8])
{
	pos in;
	printf("%s piece: ", player==PLAYER_1?"p1":"p2");
    scanf("%d-%d", &in.x, &in.y);

    char* err_msg;
    if(is_invalid_piece(in, player, board, &err_msg))
    {
    	puts(err_msg);
    	return scan_piece( player, board);
    }

    return in;
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

