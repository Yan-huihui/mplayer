#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include "cmd.h"
#include "ui.h"


char list[100][128] = {0};	//定义数组用于存放音视频列表
int list_p = 0;				//用于表示列表位置
int list_bottom = 0;		//用于表示列表末尾
int exit_t = 0;				//用于未播放状态退出时不提示错误


void clear_stdin_buff(void)
{
	char c;			
	while((c = getchar()) != '\n' && c != EOF);			//清空输入缓冲区内容
}


int error_continue(void)			//出错按任意键继续(用于清屏前显示错误信息，阻塞等待)
{
	printf("请按任意键继续...");
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );

	putchar('\n');
    return ch;
}


int get_choose(void)
{
	int choose = 0;
	int ret = scanf("%d", &choose);
	clear_stdin_buff();		//清理缓冲区防止把错误类型数据带入下一级输入
	if(ret < 1)
	{
		perror("fail to scanf in get_choose");
		error_continue();
		return -1;
	}

	switch(choose)								//由输入的值去执行不同操作
	{
		case 1:
			choose_file();break;
		case 2:
			do_play();break;
		case 3:
			do_stop();break;
		case 4:
			play_last();break;
		case 5:
			play_next();break;
		case 6:
			set_speed();break;
		case 7:
			set_seek();break;
		case 8:
			set_mode();break;
		case 9:
			do_exit();break;
		default:
			printf("请输入正确序号\n");			//输入序号错误报错
			error_continue();
			return 0;
	}
}


int get_list(void)				//查找Music目录下的影视频文件，把名称放入定义的列表中
{
	DIR *dir = opendir("/home/linux/Music");	
	if(NULL == dir)
	{
		printf("fail to opendir\n");
		error_continue();
		return -1;
	}
	while(1)
	{
		struct dirent *p = readdir(dir);
		if(NULL == p)
		{
			break;
		}

		char filename[128] = {0};
		strcpy(filename, p->d_name);

		if(filename[0] == '.')			//跳过隐藏文件
		{
			continue;
		}

		int i = strlen(filename) - 1;
		int jump = 0;

		while(filename[i] != '.')		//找到文件名中最后一个.
		{
			--i;
			if(i ==  0)					
			{
				jump = 1;	
			}
		}

		if(jump == 1)
		{
			continue;					//从文件名末尾找到头还没找到.跳过
		}

		if(strcmp(&filename[i], ".mp3") == 0 || strcmp(&filename[i], ".mp4") == 0 
			|| strcmp(&filename[i], ".avi") == 0 || strcmp(&filename[i], ".rm") == 0
			|| strcmp(&filename[i], ".flv") == 0 || strcmp(&filename[i], ".wma") == 0
			|| strcmp(&filename[i], ".rmvb") == 0)		//判断文件若为音视频文件则添加到列表
		{
			strcpy(list[list_p], filename);
			++list_p;
			++list_bottom;

			if(list_bottom == 100)			//定义的列表二维数组只有100行读到100行退出防止越界访问
			{
				list_p = 0;
				break;
			}
		}
	}
	list_p = 0;

	if(list_bottom == 0)
	{
		printf("目录下未找到音视频文件\n");
		exit(1);
	}
	closedir(dir);
	return 0;
}


int choose_file(void)							//查看播放列表
{
	printf("\033[2J");
	show_list(list,list_bottom);
	int buff;
	int ret = scanf("%d",&buff);				//得到要选择歌曲的序号
	clear_stdin_buff();							//清理缓冲区防止把错误类型数据带入下一级输入
	
	if(ret < 1)
	{
		perror("fail to scanf in choose_file");
		error_continue();
		return -1;
	}

	if(buff <= list_bottom && buff > 0)			//判断输入序号是否正确，防止越界访问
	{
		list_p = buff - 1 ;
	}
	else
	{
		printf("请输入正确序号\n");
		error_continue();
		return -1;
	}

	if(stat_g != PLAY_FREE)			//为播放或暂停状态先退出当前播放文件，再打开选择的文件
	{
		do_stop();
	}
	do_play();

}


void exit_child(int signo)			//用于回收子进程mplayer空间
{
	wait(NULL);
	stat_g = PLAY_FREE;

	if(next == NEXT_MODE)			//正常播放结束没有被打断播放结束后播放下一曲
	{
		play_next();
	}
}


int do_play(void)					//音视频播放
{
	if(stat_g == PLAY_FREE)
	{
		next = NEXT_MODE;
		stat_g = PLAY_ON;
		pid_t pid = fork();
		if(pid > 0)
		{
			signal(SIGCHLD, exit_child);		//注册信号，在子进程结束后回收子进程mplayer空间
		}
		else if(0 == pid)
		{
			close(1);
			close(2);
			char path[256] = {0};
			sprintf(path,"/home/linux/Music/%s",list[list_p]);
			execlp("mplayer", "mplayer", "-slave", "-input", "file=./fifo", path , NULL);
		}
		else
		{
			perror("fail to fork");
			error_continue();
			return -1;
		}
	}
	else
	{
		do_pause();
	}
}


int do_pause(void)					//音视频暂停
{
	if(stat_g == PLAY_ON)
	{
		stat_g = PLAY_PAUSE;
	}
	else if(stat_g == PLAY_PAUSE)
	{
		stat_g = PLAY_ON;
	}
	else
	{
		return -1;
	}
	int fp = open("./fifo", O_WRONLY);
	if(-1 == fp)
	{
		perror("fail to open");
		exit(1);
		error_continue();
	}

	write(fp, "pause\n", 6);
	close(fp);
	
	return 0;
}


int do_stop(void)			//退出mplayer
{
	next = NEXT_APPOINT;

	if(stat_g != PLAY_FREE)
	{
		int fp = open("./fifo", O_WRONLY);
		if(-1 == fp)
		{
			perror("fail to open");
			exit(1);
			error_continue();
			return -1;
		}
		write(fp, "stop\n", 5);
		close(fp);
		wait(NULL);
		stat_g = PLAY_FREE;
	}
	else if(stat_g == PLAY_FREE && exit_t == 0)	//未播放状态报错，退出时未播放不报错
	{
		printf("未播放音视频文件\n");
		error_continue();
	}
	return 0;
}


int play_last(void)								//上一曲
{
	if(mode == MODE_LIST)						//列表播放模式对定位列表的位置的值进行-1操作
	{
		if(stat_g != PLAY_FREE)
		{
			do_stop();
		}
		--list_p;

		if(list_p < 0)
		{
			list_p = list_bottom - 1;
		}
		do_play();
	}
	else if(mode == MODE_LOOP)					//循环播放模式继续播放当前位置文件
	{
		if(stat_g != PLAY_FREE)
		{
			do_stop();
		}
		do_play();
	}
	else if(mode == MODE_RANDOM)				//随机播放模式对定位列表的值取随机值
	{
		if(stat_g != PLAY_FREE)
		{
			do_stop();
		}
		
		int buff;
		do
		{
			buff = random() % list_bottom;
		}while(buff == list_p);					//让随机取的位置不等于当前位置

		list_p = buff;
		do_play();
	}

}


int play_next(void)								//下一曲
{
	if(mode == MODE_LIST)						//列表模式下对定位列表的值+1
	{
		if(stat_g != PLAY_FREE)
		{
			do_stop();
		}
		++list_p;

		if(list_p == list_bottom)
		{
			list_p = 0;
		}
		do_play();
	}
	else if(mode == MODE_LOOP)					//循环播放模式继续播放当前位置文件
	{
		if(stat_g != PLAY_FREE)
		{
			do_stop();
		}
		do_play();
	}
	else if(mode == MODE_RANDOM)				//随机播放模式对定位列表的值取随机值
	{
		if(stat_g != PLAY_FREE)
		{
			do_stop();
		}
		
		int buff;
		do
		{
			buff = random() % list_bottom;
		}while(buff == list_p);					//让随机取的位置不等于当前位置

		list_p = buff;
		do_play();
	}
}


int set_speed(void)								//设置倍速
{
	printf("\033[2J");
	show_speed();
	int set = 0;
	int ret = scanf("%d",&set);
	clear_stdin_buff();							//清理缓冲区防止把错误类型数据带入下一级输入

	if(ret < 1)
	{
		perror("fail to scanf in set_speed");	//输入格式错误报错
		error_continue();
		return 0;
	}
	
	if(stat_g == PLAY_FREE)						//未播放状态设置倍速报错
	{
		printf("在播放音视频文件后设置倍速\n");
		error_continue();
		return -1;
	}

	int fp = open("./fifo", O_WRONLY);
	switch(set)
	{
		case 1:									//设置1倍速
			write(fp, "speed_set 1\n", 12);break;
		case 2:									//设置2倍速
			write(fp, "speed_set 2\n", 12);break;
		case 3:									//设置4倍速
			write(fp, "speed_set 4\n", 12);break;
		default:
			printf("请输入正确序号\n");			//输入序号错误报错
			error_continue();
			break;
	}
	close(fp);
}


int set_seek(void)								//设置定位
{
	printf("\033[2J");
	show_seek();
	int seek = 0;
	int ret = scanf("%d",&seek);
	clear_stdin_buff();							//清理缓冲区防止把错误类型数据带入下一级输入
	
	if(ret < 1)
	{
		perror("fail to scanf in set_seek");	//输入格式错误报错
		error_continue();
		return -1;
	}

	if(stat_g == PLAY_FREE)						//未播放状态定位报错
	{
		printf("在播放音视频文件后定位\n");
		error_continue();
		return -1;
	}
	
	if(seek < 0 || seek > 100)					//输入序号错误报错
	{
		printf("请输入正确百分比\n");
		error_continue();
		return -1;	
	}

	FILE *fp = fopen("./fifo", "a");
	if(fp == NULL)
	{
		perror("fail to fopen");
		error_continue();
	}

	fprintf(fp, "seek %d 1\n", seek);			//向管道中写入定位的信息
	fclose(fp);
	return 0;
}


int set_mode(void)								//设置播放模式
{
	printf("\033[2J");
	show_play_mode();
	int mode_t = 0;
	int ret = scanf("%d",&mode_t);
	clear_stdin_buff();							//清理缓冲区防止把错误类型数据带入下一级输入
	
	if(ret < 1)									//输入格式错误报错
	{
		perror("fail to scanf in set_mode");
		error_continue();
		return 0;
	}

	switch(mode_t)								//根据输入的内容设定不同播放模式
	{
		case 1:
			mode = MODE_LIST;break;
		case 2:
			mode = MODE_LOOP;break;
		case 3:
			mode = MODE_RANDOM;break;
		default:
			printf("请输入正确序号\n");			//输入序号错误报错
			error_continue();
			break;
	}
}


int do_exit(void)								//退出整个进程（father & child）
{
	exit_t = 1;									//未播放状态退出在stop中不提示未播放状态
	do_stop();
	exit(0);
}
