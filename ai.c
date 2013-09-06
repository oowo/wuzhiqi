#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "ui.h"
#include "ai.h"

int print(char *s , int pri[15][15][4],int sum)
{
	int i,j,k;
	FILE *fp;
	fp = fopen("log.txt","a+");
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			for(k=0;k<4;k++)
			{
				fprintf(fp,"%s[%d][%d][%d] = %d  ",s,i,j,k,pri[i][j][k]);
			}
			fprintf(fp," sum = %d\n",sum);
		}
	}
	fclose(fp);
	return 0;
}

Step cpu_move(char board[15][15],int player,int cpupri[15][15][4], int userpri[15][15][4])
{

	int userplayer ; 
	int tag ; 
	int sumcpu = 0;
	int sumuser = 0;
	int row,col;
	int urow,ucol;
	int cpumax, usermax;
	int ret;
	int userd = 0;
	Step cpu[225] ={0};
	Step user[225] ={0};
	Step one;
	if(player == WHITE)
		userplayer = BLACK;
	else
		userplayer = WHITE;

	cpumax = feixicpu(board ,cpupri,player,cpu);
	usermax = fenxiuser( board,userpri,userplayer,user);
	
	if(cpumax >= usermax)
	{
		//printf("jing gong\n");
		if(0 == cpumax && board[7][7] == NOTHING)
		{
			one.row = 7;
			one.col = 7;
		}
		else
		{
			userd = fangyufenxi(board,cpu,userplayer);
			one.row = cpu[userd].row;
			one.col = cpu[userd].col;
		}
	}
	else
	{
		//printf("fang shou\n");
		userd = fangyufenxi(board,user,userplayer);
		one.row = user[userd].row;
		one.col = user[userd].col;	
	}
	one.player = player;
	return one;

}

static int fangyufenxi(char board[15][15],Step user[225],int userplayer)
{
	int i = 0;
	int player;
	int j,k,cont;
	int sum;
	int min[225] = {0};
	int max[225] = {0};
	int mininmax = 0;
	int mininmaxf[225] = {0};
	int mininmin[225] = {0};
	int mininminf[225] = {0};
	int tempri[15][15][4];
	int rd;
	if(userplayer == BLACK)
		player = WHITE;
	else
		player = BLACK;
	
	while(VALID(user[i].row,user[i].col))
	{
		board[user[i].row][user[i].col] = player;
		j=0;
		sum =0;
		while(VALID(user[j].row,user[j].col))
		{
			if(i == j)
			{
				j++;
				continue;
			}
			for(k = 0 ; k < 4 ; k++ )
			{
				bily(board,user[j].row,user[j].col,k,&cont,player);
				tempri[user[j].row][user[j].col][k] = cont; 
			}
			
			sum = yidianpingfen( tempri , user[j].row , user[j].col);
			if(j==0)
			{
				min[i] = sum;
				max[i] = sum;
			}
			if(max[i]<sum)
				max[i] = sum;
			if(min[i]>sum)
				min[i] = sum;
			j++;

		}
		board[user[i].row][user[i].col] = NOTHING;
		i++;
	}
	for(k = 0,j=0 ; k < i ; k++)
	{
		if(k == 0)			
			mininmax = max[k];
		if(mininmax > max[k])
		{
			mininmax = max[k];
			j = 0;
			mininmaxf[j] = k;
			j++;
		}
		else if(mininmax == max[k])
		{
			mininmaxf[j] = k;
			j++;
		}
	}
	srandom(time(NULL));
	rd = random()%j;
	return mininmaxf[rd];
}

static int sumpri(int aipri[15][15][4])
{
	int n = 0;
	int i,j;
	int irow,icol;
	int c[7];
	int sum = 0 ;
	int cont;
	for(i = 0;i<225;i++)
	{
		irow = i/15;
		icol = i%15;
		sum = 0;
		for(j = 0 ; j< 7 ; j++) 
			c[j] = 0;
		for(j = 0 ; j < 4 ; j++ )
		{
			cont = aipri[irow][icol][j]; 
			if(FIVE == cont )
				c[0] += 1;
			if(HFOUR == cont)
				c[1] += 1;
			if(SFOUR == cont)
				c[2] += 1;
			if(HTHREE == cont)
				c[3] += 1;
			if(STHREE == cont)
				c[4] += 1;
			if(HTWO == cont)
				c[5] += 1;
			if(STWO == cont)
				c[6] += 1;
			sum += cont;
		}
		if(c[0] >= 1)
			sum =FIVE;
		else if(c[1]>=1 || (c[2]>=1&&c[3]>=1) ||(c[2]>=2))
			sum = HFOUR;
		else if(c[3] >= 2)
			sum = 5000;
		else if(c[3] == 1 && c[4] >= 1)
			sum = 1000;
		else if(c[5] >= 2)
			sum = 100;
		n += sum;
	}
	return n ;
}

static int feixicpu(char board[15][15],int aipri[15][15][4],int player,Step cpu[225] )
{
	int col,row;
	int i , j,cont;
	int sum = 0;
	int max = 0;
	int icpu = 0;
	
	for(i = 0;i < 225;i++)
	{
		row = i/15;
		col = i%15;
		if( NOTHING != board[row][col] )
		{
			aipri[row][col][0] = 0;
			aipri[row][col][1] = 0;
			aipri[row][col][2] = 0;
			aipri[row][col][3] = 0;
			continue ;
		}

		for(j = 0 ; j < 4 ; j++ )
		{
			bily(board,row,col,j,&cont,player);
			aipri[row][col][j] = cont; 
		}
		
		sum = yidianpingfen( aipri , row , col);
		
		if(max < sum )
		{
		 	max = sum;
			icpu = 0;
			cpu[icpu].row = row;
			cpu[icpu].col = col;
			icpu++;
		}
		else if(max == sum )
		{
			cpu[icpu].row = row;
			cpu[icpu].col = col;
			icpu++;
		}

	}
	cpu[icpu].row = -1;
	cpu[icpu].col = -1;
	if(icpu <= 0)
	{
		printf("%s:%s_%d error\n",__FILE__,__FUNCTION__,__LINE__);
		return -10;
	}
	
	return max;
	
}

static int fenxiuser(char board[15][15],int aipri[15][15][4],int userplayer,Step user[225] )
{
	int i,j;
	int sum = 0;
	int row,col;
	int cont = 0;
	int max = 0;
	int k =0;
	
	for(i=0;i<225;i++)
	{
		row = i/15;
		col = i%15;
		if( NOTHING != board[row][col] )
		{
			aipri[row][col][0] = 0;
			aipri[row][col][1] = 0;
			aipri[row][col][2] = 0;
			aipri[row][col][3] = 0;
			continue ;
		}

		for(j = 0 ; j < 4 ; j++ )
		{
			bily(board,row,col,j,&cont,userplayer);
			aipri[row][col][j] = cont; 
		}

		sum = yidianpingfen(aipri, row, col);

		if(max < sum)
		{
			max = sum;
			k = 0;
			user[k].row = row;
			user[k].col = col ;
			k++;
		}
		else if(max == sum)
		{
			user[k].row = row;
			user[k].col = col ;
			k++;
		}
	}
	//print("userpri",aipri,sum);
	user[k].row = -1;
	user[k].col = -1;
	if(0 >= k )
	{
		printf("%s:%s_%d error\n",__FILE__,__FUNCTION__,__LINE__);
		return -10;
	}
	return max;

}

int yidianpingfen(int aipri[15][15][4],int row,int col) 
{
	int sum = 0;
	int i,a;
	int c[7] = {0};
	
	for(i = 0 ; i < 4 ; i++ )
	{
		a = aipri[row][col][i];
		if(FIVE == a )
			c[0] += 1;
		if(HFOUR == a)
			c[1] += 1;
		if(SFOUR == a)
			c[2] += 1;
		if(HTHREE == a)
			c[3] += 1;
		if(STHREE == a)
			c[4] += 1;
		if(HTWO == a)
			c[5] += 1;
		if(STWO == a)
			c[6] += 1;
		sum += a;
	}
	
	if(c[0] >= 1)
		sum =FIVE;
	else if(c[1]>=1 || (c[2]>=1&&c[3]>=1) ||(c[2]>=2))
		sum = HFOUR;
	else if(c[3] >= 2)
		sum = 5000;
	else if(c[3] == 1 && c[4] >= 1)
		sum = 1000;
	else if(c[5] >= 2)
		sum = 100;

	return sum;
	
}

static int bily(char board[15][15],int row,int col,int to ,int *count,int player)
{
	int i,j;
	int irow,icol;
	int inoth,iplay,iother;
	int otherplayer;
	int tempfen;
	int test[5] ={0};
	char a[8] = {'\0'};
	if(WHITE == player)
		otherplayer = BLACK;
	else
		otherplayer = WHITE;
	board[row][col] = player;
	switch (to)
	{
		case 0:
			irow = 0,icol = 1;
			break;
		case 1:
			irow = 1,icol = 0;
			break;
		case 2:
			irow = 1,icol = 1;
			break;
		case 3:
			irow = 1,icol = -1;
			break;
	}
	for(i=-4;i<=0;i++)
	{
		if(VALID(row+ (i-1)*irow, col+(i-1)*icol )&& board[row+ (i-1)*irow][ col+(i-1)*icol] == NOTHING)
		{
			a[0] = NOTHING;
		}
		else
		{
			a[0] = 'n';
		}
		if(VALID(row+ (i+5)*irow, col+(i+5)*icol) && board[row+ (i+5)*irow][ col+(i+5)*icol] == NOTHING)
		{
			a[6] = NOTHING;
		}
		else
		{
			a[6] = 'n';
		}
		if(VALID(row + i*irow, col+i*icol )&&VALID(row + (i+4)*irow, col + (i+4)*icol ))
		{

			iother = 0,iplay=0,inoth=0;
			for(j = 0 ; j<5;j++ )
			{
				a[j+1] = board[row+(i+j)*irow][col+(i+j)*icol];
				if(otherplayer == a[j+1])
					iother++;
				else if(player ==a[j+1])
					iplay++;
				else
					inoth++;
			}
		
			if(iother>0)
				test[i+4] = 0;
			else if(5 == iplay)
				test[i+4]= FIVE;
			else if(4 == iplay)
				test[i+4] = pingfenfor4(a,player);
			else if(3 == iplay)
			{
				test[i+4]= pingfenfor3(a,player);
			}
			else if(2 == iplay)
				test[i+4] = pengfenfor2(a,player);
			else
				test[i+4]= 0;
		}
	}
	*count = 0;
	for(i=0;i<5;i++)
	{
		if(*count <test[i])
			*count = test[i];
	}
	board[row][col] = NOTHING;
	return 0;
	
}


int pingfenfor3(char a[8],int player)
{
	int fenshu = 0;
	if(player == a[1] && player == a[2] && player == a[3])
	{
			if(NOTHING == a[0])
				fenshu = HTHREE;
			else
				fenshu = STHREE;
	}

	if(player == a[1] && player == a[2] && player == a[4])
	{
			if(NOTHING == a[0])
				fenshu = HTHREE;
			else
				fenshu = STHREE;
	}	

	if(player == a[1] && player == a[2] && player == a[5])
	{
				fenshu = STHREE;
	}	

	if(player == a[1] && player == a[3] && player == a[4])
	{
			if(NOTHING == a[0])
				fenshu = HTHREE;
			else
				fenshu = STHREE;
	}	

	if(player == a[1] && player == a[3] && player == a[5])
	{
				fenshu = STHREE;
	}	

	if(player == a[1] && player == a[4] && player == a[5])
	{

				fenshu = STHREE;
	}	

	if(player == a[2] && player == a[3] && player == a[4])
	{
			if(NOTHING == a[0] || NOTHING == a[6])
				fenshu = HTHREE;
			else
				fenshu = STHREE;
	}	

	if(player == a[2] && player == a[3] && player == a[5])
	{
			if(NOTHING == a[6])
				fenshu = HTHREE;
			else
				fenshu = STHREE;
	}	

	if(player == a[2] && player == a[4] && player == a[5])
	{
			if(NOTHING == a[6])
				fenshu = HTHREE;
			else
				fenshu = STHREE;
	}	

	if(player == a[3] && player == a[4] && player == a[5])
	{
			if(NOTHING == a[6])
				fenshu = HTHREE;
			else
				fenshu = STHREE;
	}

	return fenshu;
}

 int pingfenfor4(char a [ 8 ], int player)
{
	int fenshu = 0;
	
	if(NOTHING== a[1])
	{
		if(NOTHING == a[6])
			fenshu = HFOUR;
		else 
			fenshu = SFOUR;
	}
	else if(NOTHING == a[5])
	{
		if(NOTHING == a[0])
			fenshu = HFOUR;
		else 
			fenshu = SFOUR;
	}
	else
	{
		fenshu = SFOUR;
	}

	return fenshu;
	

}

 int pengfenfor2(char a [ 8 ], int player)
{
	int fenshu = 0;
	int i , j;
	for(i=1 ; i< 5 ; i++)
	{
		if(player == a[i] && player == a[i+1])
		{
			if(1==i ||4==i )
			{
				if(1==i)
					j = 0;
				if( 4==i )
					j = 6;
				if( NOTHING == a[j])
					fenshu = HTWO;
				else
					fenshu = STWO;
			}
			else
			{
				if(NOTHING == a[0] || NOTHING == a[6])
					fenshu = HTWO;
				else
					fenshu = STWO;
			}
			
		}
		
	}
	
	if(player == a[1] && player  == a[3])
	{
		if(NOTHING == a[0])
			fenshu = HTWO;
		else
			fenshu = STWO;
	}

	if(player == a[2] && player  == a[4])
	{
		if(NOTHING == a[0]||NOTHING == a[6])
			fenshu = HTWO;
		else
			fenshu = STWO;
	}

	if(player == a[3] && player  == a[5])
	{
		if(NOTHING == a[6])
			fenshu = HTWO;
		else
			fenshu = STWO;
	}

	if(player == a[1] && player  == a[4])
	{
		if(NOTHING == a[0])
			fenshu = 0;
		else
			fenshu = 0;
	}

	if(player == a[2] && player  == a[5])
	{
		if(NOTHING == a[6])
			fenshu = 0;
		else
			fenshu = 0;
	}

	if(player == a[1] && player  == a[5])
	{
		fenshu = 0;
	}
	return fenshu;
}

 
