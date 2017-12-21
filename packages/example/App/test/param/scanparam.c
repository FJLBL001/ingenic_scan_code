
#include <stdio.h>    
#include <stdlib.h>     
#include <unistd.h>     
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      
#include <termios.h>   
#include <errno.h>      
#include <string.h>
#include "scanparam.h"




/** 
 *  @brief exist param file   
 *  @param file_name    
 *  @return  == 0 exist
 *               fail 
 */  
int exist_param_file(char *file_name)
{
    return access(file_name,F_OK);
}





/** 
 *  @brief creat param file   
 *  @param file_name    
 *  @return  > 0 success
 *           < 0 fail 
 */  
int creat_param_file(char *file_name)
{

    int fd = -1;

    fd = open(file_name,O_RDWR|O_CREAT|O_EXCL);

    if(fd > 0)
    {
        return fd;
    }
    else
    {
        return -1;
    }
}

/** 
 *  @brief open param file   
 *  @param file_name    
 *  @return  > 0 success
 *           < 0 fail 
 */  
int open_param_file(char *file_name)
{

    int fd = -1;

    fd = open(file_name,O_RDWR);

    if(fd > 0)
    {
        return fd;
    }
    else
    {
        return -1;
    }
}


/** 
 *  @brief close param file fd    
 *  @param fd    
 *  @return 0 success
 *          -1 fail
 */  
int close_param_file_fd(int fd)
{

    return close(fd);
}


/** 
 *  @brief read param file   
 *  @param fd    
 *  @param buf
 *  @param buf_size
 *  @return  > 0 success
 *           < 0 fail 
 */  
int read_param_file(int fd,void *buf ,int buf_size)
{
    return read(fd,buf,buf_size);
}

/** 
 *  @brief write param file   
 *  @param fd    
 *  @param buf
 *  @param buf_size
 *  @return  > 0 success
 *           < 0 fail 
 */  
int write_param_file(int fd,void *buf ,int buf_size)
{
    return write(fd,buf,buf_size);
}


/** 
 *  @brief destory param file   
 *  @param file_name    
 *  @return  > 0 success
 *           < 0 fail 
 */  
int destory_param_file(char *file_name )
{
    return unlink(file_name);
}




int init_scan_param(void)
{
    int fd;
    if(exist_param_file(PARAM_FILE_NAME) != 0)
    {
        int fd = creat_param_file(PARAM_FILE_NAME);
        if(fd < 0)
        {
            printf("creat_param_file fail\r\n");
            return -1;
        }

        struct scan_param _scan_param;
        memset(&_scan_param,0,sizeof(_scan_param));
        _scan_param._serial_param.speed = DEFAULT_SPEED;
        _scan_param._serial_param.flow_ctrl = DEFAULT_FLOW_CTRL; 
        _scan_param._serial_param.databits = DEFAULT_DATABITS; 
        _scan_param._serial_param.stopbits = DEFAULT_STOPBITS; 
        _scan_param._serial_param.parity = DEFAULT_PARITY;

        if(write_param_file(fd,&_scan_param,sizeof(_scan_param)) != sizeof(_scan_param) )
        {
            printf("write_param_file fail\r\n");
            return -1;
        }
            
        close_param_file_fd(fd);
    }
    return 0;
}



int read_scan_param(struct scan_param *p_scan_param,int buf_size)
{
    int fd;
    fd = open_param_file(PARAM_FILE_NAME);  
    if(fd < 0)
    {
        return -1;
    }

    if(read_param_file(fd,p_scan_param,buf_size) != buf_size)
    {
        close_param_file_fd(fd);
        return -1;
    }
    close_param_file_fd(fd);
    return 0;
}