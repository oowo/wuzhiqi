#include <stdio.h>
#include <string.h>
#include "ui.h"
#include <time.h>
#include <stdlib.h>
#include "ai.h"
//打印列标题, A B C ....
static void print_col_title()
{
	int col;

	printf("   "); //3个空格
	for( col=0; col<15; col++ )
	{	
		printf(" %c", 'A'+col); 
	}
	putchar('\n');
}

//打印棋盘及棋子( . O X )
void print_board( char a[15][15] )
{
	int row, col;

	print_col_title(); //打印列标题

	//打印各行
	for( row = 0; row<15; row++ )
	{
		printf("%3d", row+1); //行号

		for( col=0; col<15; col++ )
		{
			if( 7==col && 7==row && 0 == a[row][col])
			{
				printf(" +");  // + 标记棋盘中心
				continue;
			}

			if( BLACK == a[row][col] )
				printf(" X");
			else if ( WHITE == a[row][col] )
				printf(" O");
			else
				printf(" .");
		}

		printf("%3d\n", row+1); //行号
	}

	print_col_title(); //最后一行，打印列标题
}

Step computer_move(char board[15][15],const int player)
{
	Step one;
	srandom(time(NULL));
	while(1)
	{
		one.row = random()%15;
		one.col = random()%15;
		if(board[one.row][one.col] == NOTHING)
		{
			one.player= player;
			return one;
		}
	}

}

Step player_move( char board[15][15],const int player,int cpupri[15][15][4], int userpri[15][15][4])
{
	Step one;
	int i ,j;
	int cont ;
	int p[7] ={0};
	int sum;
	char c[10] = {0};
	char crow[3]= {0};
	while(1)
	{
		fgets(c,10,stdin);

		if('p' == c[0])
		{
			if( 'c' == c[1] )
			{
				if(c[2]<='z'&&c[2]>='a')
					c[2] -= 32;
				one.col = c[2]-'A';
				memset(crow,'\0',3);
				strncpy(crow,c+3,2);
				one.row = atoi(crow)-1;
				sum=0;
				for(i=0;i<7;i++)
					p[i] = 0;
				for(i=0;i<4;i++)
				{
					printf("cpupri_%c%d_%d=%d ",c[2],one.row+1,i,cpupri[one.row][one.col][i]);
					cont = cpupri[one.row][one.col][i];
					if(FIVE == cont )
						p[0] += 1;
					if(HFOUR == cont)
						p[1] += 1;
					if(SFOUR == cont)
						p[2] += 1;
					if(HTHREE == cont)
						p[3] += 1;
					if(STHREE == cont)
						p[4] += 1;
					if(HTWO == cont)
						p[5] += 1;
					if(STWO == cont)
						p[6] += 1;
					sum += cont;
				}
				if(p[0] >= 1)
					sum =FIVE;
				else if(p[1]>=1 || (p[2]>=1&&p[3]>=1) ||(p[2]>=2))
					sum = HFOUR;
				else if(p[3] >= 2)
					sum = 5000;
				else if(p[3] == 1 && p[4] >= 1)
					sum = 1000;
				else if(p[5] >= 2)
					sum = 100;

				printf("sum = %d\n",sum);

			}
		
			if( 'u' == c[1] )
			{
				if(c[2]<='z'&&c[2]>='a')
					c[2] -= 32;
				one.col = c[2]-'A';
				memset(crow,'\0',3);
				strncpy(crow,c+3,2);
				one.row = atoi(crow)-1;
				
				sum = 0;
				for(i=0;i<7;i++)
					p[i] = 0;
				for(i=0;i<4;i++)
				{
					printf("userpri_%c%d_%d=%d ",c[2],one.row+1,i,userpri[one.row][one.col][i]);
					cont = userpri[one.row][one.col][i];
					if(FIVE == cont )
						p[0] += 1;
					if(HFOUR == cont)
						p[1] += 1;
					if(SFOUR == cont)
						p[2] += 1;
					if(HTHREE == cont)
						p[3] += 1;
					if(STHREE == cont)
						p[4] += 1;
					if(HTWO == cont)
						p[5] += 1;
					if(STWO == cont)
						p[6] += 1;
					sum += cont;
				}
				if(p[0] >= 1)
					sum =FIVE;
				else if(p[1]>=1 || (p[2]>=1&&p[3]>=1) ||(p[2]>=2))
					sum = HFOUR;
				else if(p[3] >= 2)
					sum = 5000;
				else if(p[3] == 1 && p[4] >= 1)
					sum = 1000;
				else if(p[5] >= 2)
					sum = 100;

				printf("sum = %d\n",sum);
			}
			continue;
		}
		
		if(c[0]<='z'&&c[0]>='a')
			c[0] -= 32;
		one.col = c[0]-'A';
		memset(crow,'\0',3);
		strncpy(crow,c+1,2);
		one.row = atoi(crow)-1;
		if(VALID(one.row, one.col)&&(board[one.row][one.col] == NOTHING))
		{
			one.player = player;
			return one;
		}
		printf("error\n");
	}
}


int judege(char board [ 15 ] [ 15 ], const Step* one)
{
	int count = 1;
	int a[8][2] = {{UP, NOTMOVE},{DOWN, NOTMOVE},{NOTMOVE, LEFT},{NOTMOVE, RIGHT},\
		{UP, LEFT},{DOWN, RIGHT},{UP, RIGHT},{DOWN,LEFT}};
	int  i ;
	int rowpianyi,colpianyi;
	
	for(i=0;i<8;i++)
	{
		if(i%2 == 0)
			count = 1;
		fanwei( board,  one, &count, a[i]);
		if(count >=5)
			return WINNER;
	}

	return 0;
}

static int  fanwei( char board [ 15 ] [ 15 ],const Step* one,int* count, const int pianyi[2])
{
	int row,col;
	int player;
	row = one->row;
	col = one->col;
	player = one->player;
	
	while(1)
	{
		row+=pianyi[0] ;col+=pianyi[1];
		if(VALID(row, col)&&board[row][col] == player)
			(*count)++;
		else
			break;
	}
	
}

Step receive_move(const int player)
{
	FILE * fpin = NULL;
	char buf[16] = {0};
	char crow[4] = {0};
	Step one;
	if(player == BLACK)
		fpin = fopen("black.pipe", "r");
	else
		fpin = fopen("white.pipe", "r");
	if( NULL == fpin )
	{
		fprintf(stderr, "File open failed\n");
		return ;
	}

	fgets(buf, 16, fpin);
	fclose(fpin);
	fpin = NULL;
	
	if(buf[0]<='z'&&buf[0]>='a')
	buf[0] -= 32;
	one.col = buf[0]-'A';
	memset(crow,'\0',3);
	strncpy(crow,buf+1,2);
	one.row = atoi(crow)-1;
	one.player = player ;
	
	return one;
}

int send_move(const Step one)
{
	FILE * fpout = NULL;
	char buf[16] = {0}; 
	if(one.player == WHITE)
		fpout = fopen("white.pipe","a");
	else
		fpout = fopen("black.pipe","a");
	if( NULL == fpout )
	{
		fprintf(stderr, "File open failed\n");
		return -1;
	}
	fprintf(fpout,"%c%d\n",one.col+'A',one.row+1);
	fclose(fpout);
	return  0 ;
}





