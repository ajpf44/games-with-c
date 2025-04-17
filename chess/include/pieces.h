#pragma once

typedef enum 
{
    NO_PIECE = 0,
    
    B_CODE=-1,
    
    B_PAWN=1,
    B_TOWER=2,
    B_HORSE=3,
    B_BISHOP=4,
    B_KING=5,
    B_QUEEN=6,

    W_CODE = -2,
    
    W_PAWN = 10,
    W_TOWER = 20,
    W_HORSE = 30,
    W_BISHOP = 40,
    W_KING = 50,
    W_QUEEN = 60
} piece_code_t;


