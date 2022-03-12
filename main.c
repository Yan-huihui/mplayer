#include "ui.h"
#include "cmd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
int main(int argc, const char *argv[])
{
	mkfifo("./fifo", 0777);
	
	//初始化操作
	get_list();				//获取列表			
	stat_g = PLAY_FREE;		//设置歌曲状态为未播放
	mode = MODE_LIST;		//设置播放模式为顺序播放

	while(1)
	{
		printf("\033[2J");
		show_menu();
		get_choose();
	}
	
	return 0;
}
