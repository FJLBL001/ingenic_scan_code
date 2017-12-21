#define LOG_NDEBUG 0
#define LOG_TAG "Init"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>  
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <utils/Log.h>
#include <pthread.h>
#include <fcntl.h>
#include "debugger.h"
#include "include/monitor/ProcessMonitor.h"
#include "Semaphore.h"

using namespace android;

#define SERVER_COUNT 5
const char* servers[SERVER_COUNT]{
    "/usr/bin/servicemanager",
    "/usr/bin/mediaserver",
    "/usr/bin/inputservice",
    "/usr/bin/WpaManager",
    "/usr/bin/BluetoothManager",
	};

static bool runing = true;

void thread_stop(int sig)
{
    runing = false;
    int pipe_fd=open(FIFO_NAME,O_WRONLY);    
    write(pipe_fd, "stop", strlen("stop")+1);
}

void *btThreadRun(void *me){
    system("/usr/bin/BluetoothManager");
}

void server_start(const char* const& server_name){
    ALOGV("server_start\n");
    //通过execve启动BluetoothManager,导致bsa_server_mips启动失败
    if(strcmp(server_name, "/usr/bin/BluetoothManager") == 0){
	    pthread_t tid;
	    pthread_create(&tid, NULL, btThreadRun, NULL);
	    return;
    }
    pid_t pid;
    pid = fork();
    if (pid == -1){
	ALOGE("fork failed!");
	return;
    }
    if (pid == 0) {
	execve(server_name,NULL,NULL);
    }else{
	signal(SIGCHLD,SIG_IGN);//通知内核，自己对子进程的结束不感兴趣，那么子进程结束后，内核会回收， 并不再给父进程发送信号。
    }
}

void restartServer(char* const& server_name){
    ALOGV("restartServer:%s",server_name);
    //进程名长度限制,BluetoothManager进程名为BluetoothManage
    if(strcmp(server_name,"BluetoothManage") == 0){
        server_start("/usr/bin/BluetoothManager");
	return;
    }
    char str[80];
    strcpy(str,"/usr/bin/");
    strcat(str,server_name);

    for(int i=0;i<SERVER_COUNT;i++){
//	const char* post = strstr(servers[i],server_name); //FIXME:strstr() has BUG
	if(strcmp(str,servers[i]) == 0)
	    server_start(servers[i]);
    }

}
void *threadRun(void *me) {
    int pipe_fd = 0; 
    int res;
    char buffer[PROCESS_NAME_LENGTH + 1];  
    while (runing) {
	    //ALOGV("threadRun in open");
	pipe_fd = open(FIFO_NAME, O_RDONLY);    
	res = read(pipe_fd, buffer, PROCESS_NAME_LENGTH);
	if(res > 0){
	    if(strcmp(buffer,"stop") == 0)
		break;

	    restartServer(buffer);
	}
    }
    return NULL;
}


int main(int argc, char** argv)
{
    ALOGV("%s", __FUNCTION__);

    int pipeFile =  mkfifo(FIFO_NAME, 0755); 
    if(!pipeFile){
	ALOGV("%s:create fifo ok", __FUNCTION__);
    }

    int sem_id = creatsem();
    if(sem_id == -1){
	ALOGE("creat semaphore failed");
	return 0;
    }

    for(int i=0;i<SERVER_COUNT;i++){
    	server_start(servers[i]);
    }

    pthread_t mThread;
    pthread_create(&mThread, NULL, threadRun, NULL);
    
    // Register Signal function
    if (SIG_ERR == signal(SIGINT, thread_stop)) {
	ALOGE("ERROR: signal()\n");
	return 0;
    }

    while (runing) {
	usleep(100 * 1000);
    }

    deletesem(sem_id);

    ALOGV("pthread_join");
    pthread_join(mThread, NULL);
    ALOGV("main out");

    return 0;
}
