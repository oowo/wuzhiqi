#include <stdio.h>
#include "ui.h"
#include "ai.h"
#include <unistd.h>
int main()
{
	char board[15][15]= {0};
	Step com;
	Step player;
	int count = 0;
	int iwinner = 0;
	int cpupri[15][15][4] = {0};
	int userpri[15][15][4] = {0};
	print_board( board );
	while(1)
	{
		printf("Turn player\n");
		player = player_move(board , BLACK,cpupri,userpri);
		//player = receive_move( BLACK);
		board[player.row][player.col] = player.player;
		iwinner = judege(board, &player);
		print_board( board );
		printf(">>>>>>>>>>>>>>>>>>>>>>>>\n");
		printf("heifang%c%d\n",player.col+'A',player.row+1);
		printf(">>>>>>>>>>>>>>>>>>>>>>>>\n");
		if(iwinner == WINNER)
		{
			printf("Winner is player!\n");
			break;
		}
		count++;
		if(count > 224)
		{
			printf("qi pan yi man ,qi ju jie su\n");
			break;
		}
		printf("Turn computer\n");
		com = cpu_move(board, WHITE,cpupri,userpri);
		board[com.row][com.col] = com.player;
		//send_move( com );
		iwinner = judege(board, &com);
		print_board( board );
		printf(">>>>>>>>>>>>>>>>>>>>>>>>\n");
		printf("baifang%c%d\n",com.col+'A',com.row+1);
		printf(">>>>>>>>>>>>>>>>>>>>>>>>\n");
		if(iwinner == WINNER)
		{
			printf("Winner is computer!\n");
			break;
		}
		count++;
		if(count > 224)
		{
			printf("qi pan yi man ,qi ju jie su\n");
			break;
		}


	}
	return 0;
}
