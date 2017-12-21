#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>
#include <signal.h>

#define BUTTON_EVENT "/dev/input/event0"
#define EVENT_CODE 0x74
#define DOWN 0
#define UP 1
#define YES 1
#define NO 0
#define EVENT_TYPE 1






#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)
#define BITS_PER_LONG 8

static inline int test_bit(int nr, const volatile unsigned char *addr)
{
    return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}

int main(int argc,char *argv[])
{
	int fd;
	int rd;
    int res;
	struct input_event event;
	int flags = UP;
	int sleep_flags = NO;
	time_t presstime_st=0, holdtime_st = 0 ,uptime_st=0;

    fd_set readset;
    struct timeval time_out = {0,0};


	fd = open(BUTTON_EVENT,O_RDONLY);
	if(fd < 0)
	{
		printf("Error: open %s failed!\n",BUTTON_EVENT);
	}

	printf("start ...  !\n");


	while(1)
	{
 
    /****
        FD_ZERO(&readset);
        FD_SET(fd,&readset);

        res = select(fd+1,&readset,NULL,NULL,&time_out);
        if(res == -1)
        {
            printf("select Error\n");
            goto EXIT_PROGRAME;
        }
        else if (res == 0)
        {
            printf("time out\n");
            continue;
        }
        else
        {
            if (FD_ISSET(fd,&readset))
            {
                rd = read(fd,&event,sizeof(struct input_event));


                printf("event.time.tv_sec = %d\n",event.time.tv_sec );
                printf("event.time.tv_usec = %d\n",event.time.tv_usec );
                printf("event.type = %d\n",event.type );
                printf("event.code = %d\n",event.code );
                printf("event.value = %d\n",event.value);


                if( event.code == atoi(argv[1]) )
                {

                    if (event.value == DOWN)
                    {
                        printf("event.value DOWN\r\n");

                    }
                    else
                    {
                        printf("event.value up\r\n");
                     
                    }
                }
            }
            else
            {
                printf("no read data\n");
            }
        }



		

        printf("hello world\r\n");
    *****/

    //    sleep(1);

        unsigned char key_b[KEY_MAX/8 + 1];

        memset(key_b, 0, sizeof(key_b));
        ioctl(fd, EVIOCGKEY(sizeof(key_b)), key_b);

       
        if (test_bit(116, key_b)) 
        {
            /* the bit is set in the key state */
           printf("    1.......\n");
        }
        else
        {
            printf("     2.........\n");
        }

	}

EXIT_PROGRAME:   
	close(fd);
	return 0;
}
