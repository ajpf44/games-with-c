#include <stdio.h>

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
