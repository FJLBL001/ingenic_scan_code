/* 创建信号量函数*/
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>

#define IPC_SEMKEY 1234
#if 0
#define printf printf
#define ALOGE printf
#define ALOGV printf
#define ALOGD printf
#define ALOGW printf
#define ALOGI printf
#endif

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

static int creatsem(){
    int sid;
    union semun semopts;

    if((sid = semget((key_t)IPC_SEMKEY, 1, 0666|IPC_CREAT)) == -1){
	ALOGE("create semaphore failed");
	return -1;
    }
      /*init a semaphore*/
    semopts.val = 1;
    semctl(sid, 0, SETVAL, semopts);
    ALOGI("create semaphore OK");
    return sid;
}

static int opensem(){
    int sid;
    if((sid = semget((key_t)IPC_SEMKEY, 0, 0666)) == -1){
	ALOGE("open semaphore failed");
	return -1;
    }
    return sid;
}

/* 删除信号量几*/
static int deletesem(int semid){
    return (semctl(semid, 0, IPC_RMID));
}

static int semaphore_p(int semid){
      //对信号量做减1操作，即等待P（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;//P()
    sem_b.sem_flg = SEM_UNDO;
    if(semop(semid, &sem_b, 1) == -1){
	ALOGE("semaphore_p failed\n");
        return 0;
    }
    return 1;
}

static int semaphore_v(int semid){
    //这是一个释放操作，它使信号量变为可用，即发送信号V（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;//V()
    sem_b.sem_flg = SEM_UNDO;
    if(semop(semid, &sem_b, 1) == -1){
	ALOGE("semaphore_v failed\n");
        return 0;
    }
    return 1;
}
#endif
