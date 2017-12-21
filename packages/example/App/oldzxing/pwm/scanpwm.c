#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define dbprintf(...)  do{}while(0)

//#define dbprintf printf


/**
mode :1 enable
      0 disable

**/

int enable_pwm(char mode)
{
    int fd;

    char tmp[2] ={0};

    sprintf(tmp,"%c",mode);
  
    fd = open("/sys/class/jz_pwm_dev/jz_pwm_dev0/enable", O_WRONLY);
    if (fd < 0)
    {
        dbprintf("open jz_pwm_dev0 enable fail\r\n");
        return -1;
    }

    if (write(fd, tmp,strlen(tmp)) < 0)
    {
        dbprintf("write jz_pwm_dev0 enable fail\r\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

/***
min :200

max :100000000

*****/

int set_pwm_period(char *period,int period_len)
{
    int fd; 
   
    fd = open("/sys/class/jz_pwm_dev/jz_pwm_dev0/period", O_WRONLY);
    if (fd < 0)
    {
        dbprintf("open jz_pwm_dev0 period fail\r\n");
        return -1;
    }

    if (write(fd,period,period_len) < 0)
    {
        dbprintf("write jz_pwm_dev0 period  fail\r\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}


/**

    DEV_OFF = 0,
    DEV_HALF = 127,
    DEV_FULL = 255,

**/
int set_pwm_duty_ratio(char *duty_ratio,int duty_ratio_len)
{
    int fd; 
   
    fd = open("/sys/class/jz_pwm_dev/jz_pwm_dev0/duty_ratio", O_WRONLY);
    if (fd < 0)
    {
        dbprintf("open jz_pwm_dev0 duty_ratio fail\r\n");
        return -1;
    }

    if (write(fd,duty_ratio,duty_ratio_len) < 0)
    {
        dbprintf("write jz_pwm_dev0 duty_ratio fail\r\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}