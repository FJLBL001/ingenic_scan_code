#ifndef _SCANPARAM_H
#define _SCANPARAM_H


#ifdef __cplusplus
extern "C" {
#endif


#define PARAM_FILE_NAME     "scan_param.ini"




#define DEFAULT_SPEED       9600
#define DEFAULT_FLOW_CTRL   0
#define DEFAULT_DATABITS    8
#define DEFAULT_STOPBITS    1
#define DEFAULT_PARITY      'N'



#pragma pack (1)
struct  serial_param
{
    int speed;
    int flow_ctrl; 
    int databits; 
    int stopbits; 
    int parity;
};


struct scan_param
{
    struct serial_param _serial_param;
    
};












#pragma pack()


extern int exist_param_file(char *file_name);
extern int creat_param_file(char *file_name);
extern int open_param_file(char *file_name);
extern int close_param_file_fd(int fd);
extern int read_param_file(int fd,void *buf ,int buf_size);
extern int write_param_file(int fd,void *buf ,int buf_size);
extern int destory_param_file(char *file_name );

extern int init_scan_param(void);
extern int read_scan_param(struct scan_param *p_scan_param,int buf_size);

#ifdef __cplusplus
}
#endif


#endif
