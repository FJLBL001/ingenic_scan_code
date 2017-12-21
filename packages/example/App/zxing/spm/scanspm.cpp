
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>
#include <signal.h>

#include "scanspm.h"


#define dbprintf(...)			do{}while(0)

//#define dbprintf printf
#define BUTTON_EVENT			"/dev/input/event0"


#define BITS_PER_LONG			8
#define BIT_WORD(nr)			((nr) / BITS_PER_LONG)


static inline int test_bit(int nr, const volatile unsigned char * addr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG - 1)));
}


static int		g_button_fd = 0;

static unsigned char key_b[KEY_MAX / 8 + 1];




/*******************************************************************
* 名称： 				 
* 功能： 	打开按键设备
* 出口参数: 	 return fd
*******************************************************************/
int open_key_button_event()
{
	g_button_fd = open(BUTTON_EVENT, O_RDONLY);

	if (g_button_fd < 0)
	{
		return - 1;
	}

	return g_button_fd;
}


/*******************************************************************
* 名称： 				 
* 功能： 	  检验按键当前处于什么状态(非诸塞)
* 入口参数：		 key  按键值,具体参考内核<linux/input.h>
* 出口参数: 	 return 0被按下(低电平) 1释放按键(高电平) -1(获取失败)
*******************************************************************/
int key_button_state(int key)
{
	memset(key_b, 0, sizeof(key_b));

	if (ioctl(g_button_fd, EVIOCGKEY(sizeof(key_b)), key_b) == -1)
	{
		return -1;
	}

	return test_bit(key, key_b);
}



/*******************************************************************
* 名称：                
* 功能： 读取按键的值(诸塞)配合 多路i/o复用
* 入口参数：      key  按键值,具体参考内核<linux/input.h>
* 出口参数:      return 0被按下(低电平) 1释放按键(高电平) -1(获取失败)
*******************************************************************/
int read_button_state(int key)
{
    int res;
    struct input_event event;
    res = read(g_button_fd,&event,sizeof(struct input_event));
    if(res == -1)
    {
        return -1;
    }

    if (event.code == key)
    {
        return event.value;
    }

    return -1;
}

/*******************************************************************
* 名称： 				 
* 功能：  关闭按键设备
*******************************************************************/
void close_key_button_event()
{
	close(g_button_fd);
}








