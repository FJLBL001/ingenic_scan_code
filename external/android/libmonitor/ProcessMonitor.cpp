#define LOG_NDEBUG 0
#define LOG_TAG "ProcessMonitor"

#include <utils/Log.h>
#include "Semaphore.h"
#include "include/monitor/ProcessMonitor.h"

void* shm = NULL;
int shm_id = 0;
int sem_id = 0;

bool getNameByPID(const int pid,char* const& procname){
    ALOGV("getNameByPID in pid=%d",pid);

      /*NOTICE:
      * if a process is created by fork..exec().
      *then the process name can not get in /proc/%d/cmdline
      **/
    int bufsize = 1024;
    char proc_pid_path[bufsize];
    char buf[bufsize];
    char name[50];
    sprintf(proc_pid_path, "/proc/%d/status", pid);
    FILE* fp = fopen(proc_pid_path, "r");
    if(NULL != fp){
        if( fgets(buf, bufsize-1, fp)== NULL ){
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", name);
	ALOGV("buf=%s name=%s",buf,name);
	strcpy(procname,name);
    	return true;
    }
    return false;
}
void* createShm(int& shmid){
    ALOGV("createShm");
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
      //int shmid;

      //srand((unsigned int)getpid());
    shmid = shmget((key_t)1234, SHARE_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
	ALOGE("shmget failed\n");
	return NULL;
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
	ALOGE("shmat failed\n");
	return NULL;
    }
    sem_id = opensem();
    ALOGV("createShm out addr=%p",shared_memory);
    return shared_memory;
}

int destoryShm(void* sm,int shmid){
    if (shmdt(sm) == -1) {
	ALOGE("shmdt failed\n");
	return -1;
    }
    
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
	ALOGE("shmctl(IPC_RMID) failed\n");
	return -1;
    }
    
    return 0;
}

int addProcess(void*& sm,int16_t pid){
    ALOGV("addProcess pid(%d) in",pid);
    LISTPROC* listPro = (LISTPROC*)sm;
      //PROC_INFO* procInfo = listPro->proc_info;

    char name[PROCESS_NAME_LENGTH]={""};
    getNameByPID(pid,name);

    semaphore_p(sem_id);

    for(int i=0;i<PROCESS_COUNT;i++){
	if(listPro[i].pid == 0){
	    listPro[i].pid = pid;
	    strcpy(listPro[i].name,name);
	    semaphore_v(sem_id); //释放信号量
	    ALOGV("addProcess out i=%d sizeof=%d",i,sizeof(PROC_INFO));
	    return 0;
	}
    }
    semaphore_v(sem_id); //释放信号量
    ALOGE("No enough share buffer!!");
    return -1;
}

int removeProcess(int16_t pid){
    ALOGV("removeProcess pid(%d) in",pid);
    LISTPROC* listPro = (LISTPROC*)shm;
    semaphore_p(sem_id);
    for(int i=0;i<PROCESS_COUNT;i++){
	if(pid == listPro[i].pid){
	    memset(&listPro[i],0,sizeof(PROC_INFO));
	    ALOGV("removeProcess out i=%d",i);
	    semaphore_v(sem_id);
	    return 0;
	}
    }
    semaphore_v(sem_id);
    ALOGE("removeProcess failed!");
    return -1;
}

int16_t setProcessStatus(int16_t status){
    if(shm == NULL){
    	ALOGE("you have not ad a ShardBuffer!");
	return -1;
    }

    semaphore_p(sem_id);
    LISTPROC* listPro = (LISTPROC*)shm;
    int16_t pid = getpid();

    for(int i=0;i<PROCESS_COUNT;i++){
	if(pid == listPro[i].pid){
	    listPro[i].status = status;
	    ALOGV("setProcessStatus out i=%d",i);
	    semaphore_v(sem_id);
	    return 0;
	}
    }
    semaphore_v(sem_id);
    ALOGE("setProcessStatus failed!");
    return -1;
}

int16_t getProcessPID(const char* name){
    if(shm == NULL){
    	ALOGE("you have not ad a ShardBuffer!");
	return -1;
    }
    semaphore_p(sem_id);
    LISTPROC* listPro = (LISTPROC*)shm;

    for(int i=0;i<PROCESS_COUNT;i++){
	if(strcmp(name,listPro[i].name) == 0){
	    ALOGV("getProcessPID out i=%d",i);
	    semaphore_v(sem_id);
	    return listPro[i].pid;
	}
    }
    semaphore_v(sem_id);
    ALOGE("getProcessPID failed!");
    return -1;
}

int16_t getProcessStatus(const char* name){
    if(shm == NULL){
    	ALOGE("you have not ad a ShardBuffer!");
	return -1;
    }
    semaphore_p(sem_id);
    LISTPROC* listPro = (LISTPROC*)shm;

    for(int i=0;i<PROCESS_COUNT;i++){
	if(strcmp(name,listPro[i].name) == 0){
	    ALOGV("getProcessPID out i=%d",i);
	    semaphore_v(sem_id);
	    return listPro[i].status;
	}
    }
    semaphore_v(sem_id);
    ALOGE("getProcessStatus failed!");
    return STATUS_INVALID;
}
