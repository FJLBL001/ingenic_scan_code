#ifndef _SCANSERIAL_H
#define _SCANSERIAL_H


//串口相关的头文件
#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <string.h>
#include "scanparam.h"



//宏定义
#define FALSE  -1
#define TRUE   0



//#define dbprintf(...) do{}while(0)
#define dbprintf  printf




#ifdef __cplusplus
extern "C" {
#endif



/*******************************************************************
* 名称：                  UART0_Open
* 功能：                打开串口并返回串口设备文件描述
* 入口参数：        fd    :文件描述符     port :串口号(ttyS0,ttyS1,ttyS2)
* 出口参数：        正确返回为1，错误返回为0
*******************************************************************/
extern int UART0_Open(char* port);


/*******************************************************************
* 名称：                UART0_Close
* 功能：                关闭串口并返回串口设备文件描述
* 入口参数：        fd    :文件描述符     port :串口号(ttyS0,ttyS1,ttyS2)
* 出口参数：        void
*******************************************************************/

extern void UART0_Close(int fd);



/*******************************************************************
* 名称：                UART0_Set
* 功能：                设置串口数据位，停止位和效验位
* 入口参数：        fd        串口文件描述符
*                              speed     串口速度
*                              flow_ctrl   数据流控制
*                           databits   数据位   取值为 7 或者8
*                           stopbits   停止位   取值为 1 或者2
*                           parity     效验类型 取值为N,E,O,,S
*出口参数：          正确返回为1，错误返回为0
*******************************************************************/
extern int UART0_Set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity);



/*******************************************************************
* 名称：                  UART0_Recv
* 功能：                接收串口数据
* 入口参数：        fd                  :文件描述符
*                              rcv_buf     :接收串口中数据存入rcv_buf缓冲区中
*                              data_len    :一帧数据的长度
* 出口参数：        正确返回为1，错误返回为0
*******************************************************************/
extern int UART0_Recv(int fd, char *rcv_buf, int data_len);


/********************************************************************
* 名称：                  UART0_Send
* 功能：                发送数据
* 入口参数：        fd                  :文件描述符
*                              send_buf    :存放串口发送数据
*                              data_len    :一帧数据的个数
* 出口参数：        正确返回为1，错误返回为0
*******************************************************************/
extern int UART0_Send(int fd, char *send_buf, int data_len);





extern int scan_UART0_Send(int fd, char *send_buf, int data_len);
extern int scan_UART0_Send1(int fd, char *send_buf, int data_len);
extern int scan_deal_uart_msg(int fd,char *uart_msg);



#ifdef __cplusplus
}
#endif

#endif
