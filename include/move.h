#include "pieces.h"

typedef struct{
    int x;
    int y;
} pos;

typedef enum
{
    CANNOT_MOVE,
    CAN_MOVE,
    CAN_CAPTURE
} move_result_t;

move_result_t analize_move(pos actual, pos desired, int board[8][8], piece_code_t PIECE_CODE, piece_code_t TEAM_CODE);
void move_piece(pos actual, pos desired, int board[8][8], piece_code_t piece_code);

