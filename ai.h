#ifndef __AI_H__
#define __AI_H__

#ifdef __cplusplus
	extern "C" {
#endif

#define PRIUPDOWN 1
#define PRILEFTRIGHT 1
#define PRINULL 0

#define FIVE 100000
#define HFOUR 10000
#define SFOUR 500
#define HTHREE 200
#define STHREE 50
#define HTWO 5
#define STWO 3

Step cpu_move(char board[15][15],int player,int cpupri[15][15][4], int userpri[15][15][4]);
static int fangyufenxi(char board[15][15],Step user[225],int userplayer);
static Step zuiyou(char board[15][15],int aipri[15][15][4],int player,int tag);
static int sumpri(int aipri[15][15][4]);
static int feixicpu(char board[15][15],int aipri[15][15][4],int player,Step cpu[225]  );
int yidianpingfen(int aipri[15][15][4],int row,int col) ;
static int fenxiuser(char board[15][15],int aipri[15][15][4],int userplayer,Step user[225] );
static int bily(char board[15][15],int row,int col,int to ,int *count,int player);
 int pingfenfor3(char a[8],int player);
 int pingfenfor4(char a[8],int player);
 int pingfenfor2(char a[8],int player);

#ifdef __cplusplus
}
#endif
#endif
