#ifndef SHAREBUFFER_H
#define SHAREBUFFER_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

#define SHARE_SIZE 2048    //SHARE_SIZE >= (PROCESS_COUNT*sizeof(PROC_INFO))
#define PROCESS_COUNT 50
#define PROCESS_NAME_LENGTH 28

enum{
    STATUS_INVALID = 0x0,
    STATUS_PAUSE = 0x01,
    STATUS_INCALL = 0x02,
    STATUS_VIDEO_RECORDING = 0x03,           //常规录像中
    STATUS_TIMELAPSE_VIDEO_RECORDING = 0x04, //延时录像中
};

struct PROC_INFO{
    int16_t pid;
    int16_t status;    
    char name[PROCESS_NAME_LENGTH];
};

#define LISTPROC struct PROC_INFO
/*
 *setProcessStatus:set current process status
 *status:value of STATUS_XX
 *@return : 0 success. -1 failed
*/
int16_t setProcessStatus(int16_t status);

/*
 *getProcessStatus :get a process status by process name
 *name: a process name
 *@return : value of STATUS_XX
*/
int16_t getProcessStatus(const char* name);

/*
 *getProcessPID :get a process id by process name
 *name: a process name
 *@return : valid id will be return if success. then -1 failed
*/
int16_t getProcessPID(const char* name);

/*
 *removeProcess :remove a process by the process id
 *pid: a process id

 *@return :0 will be return if success. then -1 failed
*/
int removeProcess(int16_t pid);
/*
 *getNameByPID :get a process name by the process id
 *pid: a process id

 *@return :true will be return if success. and procname will be written.
 *         then false failed
*/
bool getNameByPID(const int pid,char* const &procname);

#endif
