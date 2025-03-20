#include <stdio.h>
#include <stdbool.h>

typedef struct{
    int x;
    int y;
} pos;

typedef enum 
{
    NO_PIECE = 0,
    
    B_CODE=0,
    B_PAWN=1,
    B_TOWER=2,
    B_HORSE=3,
    B_BISHOP=4,
    B_KING=5,
    B_QUEEN=6,

    W_CODE = 1,
    W_PAWN = 10,
    W_TOWER = 20,
    W_HORSE = 30,
    W_BISHOP = 40,
    W_KING = 50,
    W_QUEEN = 60
} piece_code_t;

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


int main (int argc, char** argv)
{
    void output_board(int board[8][8]);
    bool handle_move(pos actual, pos desired, int board[8][8], piece_code_t PIECE_CODE, piece_code_t TEAM_CODE);

	puts("ajpf's chess, version 0.1");

	int board[8][8];

	set_board_init_pices(board);

	board [6][0] = NO_PIECE;
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

        bool moved = handle_move(piece, desired, board, board[piece.x][piece.y], is_player1_round?W_CODE:B_CODE);

        if(moved)
        {
            puts("moving");
            // is_player1_round = !is_player1_round;
        }
        else
            puts("cannot move");
        puts("##################################");
    }

    output_board(board);
	return 0;
}
int abs(int x)
{
    if(x < 0) return (-1)*x;
    return x;
}

void move_piece(pos actual, pos desired, int board[8][8], piece_code_t PIECE_CODE)
{
    board[desired.x][ desired.y] = PIECE_CODE;
    board[actual.x][actual.y] = NO_PIECE;
}

bool can_pawn_move(pos actual, pos desired, int board[8][8], int team_code)
{
    if(board[desired.x][desired.y] != NO_PIECE  || desired.y != actual.y || abs(desired.x - actual.x) > 2)
        return false;

    if(team_code == B_CODE && actual.x == 1 && desired.x - actual.x >= 1)
        return true;
    
    if(team_code == W_CODE && actual.x == 6 && actual.x - desired.x > 0)
        return true;
    
    return false;
}

bool can_tower_move(pos actual, pos desired, int board[8][8], piece_code_t TEAM_CODE)
{
    //only can move in lines rule
    if ( desired.x != actual.x && desired.y != actual.y) return false;

    //only can move in lines rule to free space rule
    if( board[desired.x][desired.y] != NO_PIECE) return false;
    
    //only can move in lines that is free
    if(desired.x == actual.x)
    {
        int distance = abs(desired.y- actual.y);
        int minor = desired.y > actual.y?actual.y:desired.y;
        for(int i = minor+1; i < minor+distance; ++i)
        {
            if(board[actual.x][i] != NO_PIECE)
            {
                printf("\n  x: %d, y: %d, piece on spot: %d \n", actual.x, i, board[actual.x][i]);
                return false;
            }
        }
    }
    else //already was checked that desired.y == actual.y in the first if statement
    {
        int distance = abs(desired.x- actual.x);
        int minor = desired.x > actual.x?actual.x:desired.x;

        for(int i = minor+1; i < minor+distance; ++i)
        {
            if(board[i][actual.y] != NO_PIECE)
            {
                printf("\nx: %d, y: %d, piece on spot: %d \n", i, actual.y, board[i][actual.y]);
                return false;
            }
        }
    }

    return true;    
}

bool can_horse_move(pos act, pos des, int board[8][8])
{
    if(board[des.x][des.y] != NO_PIECE)
        return false;

    int delta_x = abs(act.x - des.x);
    int delta_y = abs(act.y - des.y);

    if(((delta_x*delta_x)+(delta_y*delta_y)) == 5) return true; //pitagoras teorem
    
    return false;
}

bool can_bishop_move(pos act, pos des, int board[8][8])
{
    if(board[des.x][des.y] != NO_PIECE)
        return false;

    int delta_x = act.x - des.x;
    int delta_y = act.y - des.y;
    //move only in diagonally
    if(abs(delta_y) != abs(delta_x)) return false; 

    //check if the diagonal choosed is availabe
    int iter_x = 0;
    int iter_y = 0;
    puts("//check if the diagonal choosed is availabe")
/* WIP -  WORK IN PROGRESS HERE, I'M WRITING THIS DEBUGGINS PRITNS AND TESTING THIS FUNCTION' */    
    for( i = 1; i < abs(delta_y); ++i )
    {
        iter_x += delta_x>0?1:-1;
        iter_y += delta_y>0?1:-1;
        if( board[act.x + iter_x][act.y + iter_y] != NO_PIECE) return false;
    }
    

    return true;
}

bool handle_move(pos actual, pos desired, int board[8][8], piece_code_t PIECE_CODE, piece_code_t TEAM_CODE)
{

    /* debug prints  */    
    printf("the piece_code, on handle move function: %d\n", PIECE_CODE);
    printf("piece: x = %d, y = %d\n",actual.x, actual.y);
    printf("desired: x = %d, y = %d\n",desired.x, desired.y);
    printf("piece_code: %d\n", PIECE_CODE);
    
    switch (PIECE_CODE) 
    {
        case B_PAWN:case W_PAWN:
            if(can_pawn_move(actual, desired, board, TEAM_CODE))
            {
                move_piece(actual, desired, board, PIECE_CODE);
                return true;
            }
            break;
        case B_TOWER:case W_TOWER:
            if(can_tower_move(actual, desired, board, TEAM_CODE))
            {
                move_piece(actual, desired, board, PIECE_CODE);
                return true;
            }
            break;
        case B_HORSE:case W_HORSE:
            if(can_horse_move(actual, desired, board))
            {
                move_piece(actual, desired, board, PIECE_CODE);
                return true;
            }
            break;
        case B_BISHOP:case W_BISHOP:
            if(can_bishop_move(actual, desired, board))
            {
                move_piece(actual, desired, board, PIECE_CODE);
                return true;
            }
            break;
        default:
            puts("this piece is no available for handling move yet");
            break;
    }

    return false;
}



void output_board(int board[8][8])
{
    printf("P1  ");
    for( int i = 0;i  < 8; ++i)
        printf("%2d ", i);
    

    printf("\n   -----------------------\n");
    for( int i = 0;i  < 8; ++i)
	{
        printf("%d | ", i);
		for(int j = 0; j < 8; ++j)
		{
			printf("%2d ", board[i][j]);
		}
		printf("\n");
	}
}

