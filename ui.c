#include <stdio.h>

void show_menu(void)
{
	printf("+-----------------------+\n");
	printf("|     mplayer播放器     |\n");			
	printf("+-----------------------+\n");
	printf("| 1.查看播放列表        |\n");
	printf("| 2.开始/暂停           |\n");
	printf("| 3.停止                |\n");
	printf("| 4.上一个              |\n");
	printf("| 5.下一个              |\n");
	printf("| 6.倍速播放            |\n");
	printf("| 7.定位                |\n");
	printf("| 8.播放方式            |\n");
	printf("| 9.退出                |\n");
	printf("+-----------------------+\n");
	printf("选择功能：");
	fflush(stdout);
}

void show_list(char (*p)[128], int list_bottom)
{
	printf("+-----------------------+\n");
	printf("|     mplayer播放器     |\n");			
	printf("+-----------------------+\n");
	int i = 0;
	while(i < list_bottom)
	{
		printf("|%2d.%-20s|\n", ++i, p[i]);			
	}
	printf("+-----------------------+\n");
	printf("选择歌曲：");
	fflush(stdout);
}

void show_speed(void)
{
	printf("+-----------------------+\n");
	printf("|     mplayer播放器     |\n");			
	printf("+-----------------------+\n");
	printf("| 1. 1倍速              |\n");			
	printf("| 2. 2倍速              |\n");			
	printf("| 3. 4倍速              |\n");			
	printf("+-----------------------+\n");
	printf("选择倍速：");
	fflush(stdout);
}

void show_seek(void)
{
	printf("+-----------------------+\n");
	printf("|     mplayer播放器     |\n");			
	printf("+-----------------------+\n");
	printf("定位：");
	fflush(stdout);

}

void show_play_mode(void)
{
	printf("+-----------------------+\n");
	printf("|     mplayer播放器     |\n");			
	printf("+-----------------------+\n");
	printf("| 1.顺序播放            |\n");			
	printf("| 2.单曲循环            |\n");			
	printf("| 3.随机播放            |\n");			
	printf("+-----------------------+\n");
	printf("选择模式：");
	fflush(stdout);
}
