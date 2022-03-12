#ifndef __CMD_
#define __CMD_

typedef enum
{
	PLAY_FREE,		//未播放状态
	PLAY_ON,		//播放状态
	PLAY_PAUSE,		//暂停状态
}PLAY_STAT_t;

typedef enum
{
	MODE_LIST,		//顺序播放
	MODE_LOOP,		//单曲循环
	MODE_RANDOM,	//随机播放
}PLAY_MODE;

typedef enum		//枚举按那种方式进行下一首
{
	NEXT_APPOINT,	//一首歌播放过程中打断播放下一首
	NEXT_MODE,		//一首歌播放完按照模式播放下一首
}HOW_NEXT;

HOW_NEXT next;
PLAY_STAT_t stat_g;
PLAY_MODE mode;

extern void clear_stdin_buff(void);
extern int error_continue(void);
extern int get_choose(void);
extern int get_list(void);
extern int choose_file(void);
extern void exit_child(int);
extern int do_play(void);
extern int do_pause(void);
extern int do_stop(void);
extern int play_last(void);
extern int play_next(void);
extern int set_speed(void);
extern int set_seek(void);
extern int set_mode(void);								//设置播放模式
extern int do_exit(void);


#endif
