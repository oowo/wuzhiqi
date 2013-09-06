#ifndef __CHESS_UI_H__
#define __CHESS_UI_H__

#ifdef __cplusplus
extern "C" {
#endif


#define NOTHING 0
#define BLACK   1
#define WHITE   2
#define WINNER   1
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
#define NOTMOVE 0
#define VALID(row,col) (((row)>=0&&(row)<15&&(col)>=0&&(col)<15)?1:0)

typedef struct step
{
	int player;
	int row ;
	int col ;
}Step;

void print_board( char board[15][15] );
Step computer_move( char board[15][15], const int player);
Step player_move( char board[15][15], const int player,int cpupri[15][15][4], int userpri[15][15][4]);
int  judege( char board[15][15], const Step* one);
static int  fanwei(char board [ 15 ] [ 15 ], const Step* one,int* count, const int pianyi[2]);
Step receive_move(const int player);
int send_move(const Step one);

#ifdef __cplusplus
}
#endif

#endif
