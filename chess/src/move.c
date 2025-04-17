#include "move.h"
#include "pieces.h"
#include "utils.h"

#define bool int
#define true 1
#define false 0

void move_piece(pos actual, pos desired, int board[8][8], piece_code_t piece_code)
{
    board[desired.x][ desired.y] = piece_code;
    board[actual.x][actual.y] = NO_PIECE;
}

bool can_capture(pos act, pos des, int board[8][8], piece_code_t team_code)
{
    piece_code_t target = board[des.x][des.y];
        
    if (team_code == B_CODE)
    {
        if(target < W_PAWN)
            return false;
        else
            return true;
    }

    if (team_code == W_CODE)
    {
        if(target >=  W_PAWN)
            return false;
        else
            return true;
    }
    
    return false;
}

bool can_pawn_capture(pos act, pos des, int board[8][8], piece_code_t team_code)
{
    //must capture diagonally
    if(abs(des.x - act.x) != 1 || abs(des.y - act.y) != 1)
        return false;

    //only foward diagonnaly
    if(team_code == B_CODE)
    {
        if(des.x <= act.x)
            return false;
    }

    if(team_code == W_CODE)
    {
        if(des.x >= act.x)
            return false;
    }

     return can_capture(act, des, board, team_code);
}
move_result_t can_pawn_move(pos actual, pos desired, int board[8][8], piece_code_t team_code)
{
    if(can_pawn_capture(actual, desired, board, team_code))
    {
        return CAN_CAPTURE;
    }
    
    if( board[desired.x][desired.y] != NO_PIECE)
        return CANNOT_MOVE;
    
    if(desired.y != actual.y || abs(desired.x - actual.x) > 2)
        return CANNOT_MOVE;

    if(team_code == B_CODE && desired.x - actual.x < 0) 
        return CANNOT_MOVE;

    if(team_code == W_CODE && actual.x - desired.x < 0 )
        return CANNOT_MOVE;

    if(team_code == B_CODE && actual.x != 1 && abs(desired.x - actual.x) >= 2 )
        return CANNOT_MOVE;
        
    if(team_code == W_CODE && actual.x != 6 && abs(desired.x - actual.x) >= 2 ) 
        return CANNOT_MOVE;
    
    
        
    return CAN_MOVE;
}

move_result_t can_tower_move(pos actual, pos desired, int board[8][8], piece_code_t team_code)
{
    //only can move in lines rule
    if ( desired.x != actual.x && desired.y != actual.y) 
        return CANNOT_MOVE;
    
    //only can move in lines that is free
    if(desired.x == actual.x)
    {
        int distance = abs(desired.y- actual.y);
        int minor = desired.y > actual.y?actual.y:desired.y;
        for(int i = minor+1; i < minor+distance; ++i)
        {
            if(board[actual.x][i] != NO_PIECE)
            {
                // printf("\n  x: %d, y: %d, piece on spot: %d \n", actual.x, i, board[actual.x][i]);
                return CANNOT_MOVE;
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
                // printf("\nx: %d, y: %d, piece on spot: %d \n", i, actual.y, board[i][actual.y]);
                return CANNOT_MOVE;
            }
        }
    }
    
    if( board[desired.x][desired.y] != NO_PIECE)
    {
        if(can_capture(actual, desired, board, team_code))
            return CAN_CAPTURE;
        else
            return CANNOT_MOVE;
    }

    return true;    
}

move_result_t can_horse_move(pos act, pos des, int board[8][8], piece_code_t team_code)
{
    int delta_x = abs(act.x - des.x);
    int delta_y = abs(act.y - des.y);
    //pitagoras teorem
    if(((delta_x*delta_x)+(delta_y*delta_y)) != 5) 
        return CANNOT_MOVE; 

    //only can move to free space rule
    if( board[des.x][des.y] != NO_PIECE)
    {
        if(can_capture(act, des, board, team_code))
            return CAN_CAPTURE;
        else
            return CANNOT_MOVE;
    }
    
    return CAN_MOVE;
}

move_result_t can_bishop_move(pos act, pos des, int board[8][8], piece_code_t team_code)
{
    int delta_x = act.x - des.x;
    int delta_y = act.y - des.y;
    //move only in diagonally
    if(abs(delta_y) != abs(delta_x)) 
        return CANNOT_MOVE; 

    //check if the diagonal choosed is availabe
    int iter_x = 0;
    int iter_y = 0;
    // puts("//check if the diagonal choosed is availabe");
    for(int i = 1; i < abs(delta_y); ++i )
    {
        iter_x += delta_x<0?1:-1;
        iter_y += delta_y<0?1:-1;
        // printf("checking x= %d, y= %d\n",act.x + iter_x ,act.y + iter_y );
        if( board[act.x + iter_x][act.y + iter_y] != NO_PIECE) 
            return CANNOT_MOVE;
    }

    //only can move to free space rule
    if( board[des.x][des.y] != NO_PIECE)
    {
        if(can_capture(act, des, board, team_code))
            return CAN_CAPTURE;
        else
            return CANNOT_MOVE;
    }
    
    return CAN_MOVE;
}
move_result_t can_queen_move(pos act, pos des, int board[8][8], piece_code_t team_code)
{
    //it's simple, the queen has bishop+tower movement, so the veryfing relly on that fact
    move_result_t bishop_res = can_bishop_move(act, des, board, team_code);
    move_result_t tower_res = can_tower_move(act, des, board, team_code);

    move_result_t queen_res = bishop_res==CANNOT_MOVE?tower_res:bishop_res;
        
    return queen_res;
}
move_result_t can_king_move(pos act, pos des, int board[8][8], piece_code_t team_code)
{
    //i'm not checking checkmate limiters right 

    //can only move one space, don't matter the direction
    int abs_delta_x = abs(act.x - des.x);
    int abs_delta_y = abs(act.y - des.y);

    //pitagoras teorem
    if(((abs_delta_x*abs_delta_x)+(abs_delta_y*abs_delta_y)) > 2) 
        return CANNOT_MOVE; 

    //only can move to free space rule
    if( board[des.x][des.y] != NO_PIECE)
    {
        if(can_capture(act, des, board, team_code))
            return CAN_CAPTURE;
        else
            return CANNOT_MOVE;
    }
    
    return CAN_MOVE;
}

move_result_t analize_move(pos actual, pos desired, int board[8][8], piece_code_t PIECE_CODE, piece_code_t TEAM_CODE)
{
    /* debug prints  */    
    // printf("the piece_code, on handle move function: %d\n", PIECE_CODE);
    // printf("piece: x = %d, y = %d\n",actual.x, actual.y);
    // printf("desired: x = %d, y = %d\n",desired.x, desired.y);
    // printf("piece_code: %d\n", PIECE_CODE);
    
    switch (PIECE_CODE) 
    {
        case B_PAWN:case W_PAWN:
            return can_pawn_move(actual, desired, board, TEAM_CODE);
            break;
        case B_TOWER:case W_TOWER:
            return can_tower_move(actual, desired, board, TEAM_CODE);
            break;
        case B_HORSE:case W_HORSE:
            return can_horse_move(actual, desired, board, TEAM_CODE);
            break;
        case B_BISHOP:case W_BISHOP:
            return can_bishop_move(actual, desired, board, TEAM_CODE);
            break;
        case B_QUEEN:case W_QUEEN:
            return can_queen_move(actual, desired, board, TEAM_CODE);
            break;
        case B_KING:case W_KING:
            return can_king_move(actual, desired, board, TEAM_CODE);
            break;
        default:
            // puts("this piece is no available for handling move");
            break;
    }

    return CANNOT_MOVE;
}
