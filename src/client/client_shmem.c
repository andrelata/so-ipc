#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"
#include "./client_shmem.h"

int fdS;
int myPID;
request_t *reqS;
request_t *reqC;
static sem_t *sd;

int openSChannel(){
	if ( (fdS = shm_open(SERVER_NAME, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fdS, SIZE);
	if ( !(reqS = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fdS, 0)) )
		fatal("mmap");
	close(fdS);
	reqC = reqS + 1;
	initmutex();
	myPID = getpid();

	return 0;
}

int openCChannel(){
	request_t req;
	req = receiveRequest();
	if(req.reqID != ANS){
		printf("error\n");
		return ERROR;
	}
	char pid[20];
	sprintf(pid, "%d\0", req.par1);
	char serverPath2[20] = SERVER_NAME;
	char * serverPath = strcat(serverPath2, pid);
	
	if ( (fdS = shm_open(serverPath, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fdS, SIZE);
	if ( !(reqS = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fdS, 0)) )
		fatal("mmap");
	close(fdS);
	//reqC = reqS+1;
	return OK;
}

request_t 
receiveRequest(){
	int flag = 1;
	request_t request;

	while ( flag )
	{
		enter();
		if ( reqC->reqID != EMPTY && reqC->PID == myPID)
		{
			request.price = reqC->price;
			memcpy(&request, reqC, sizeof request);
			reqC->reqID = EMPTY;
			flag = 0;
		}
		leave();
		sleep(1);
	}
	
	return request;
}


void 
sendRequest(request_t request){
	int flag = 1;

	while(flag){
		enter();
		if(reqS->reqID == EMPTY ){
			reqS->price = request.price;
			memcpy(reqS, &request, sizeof request);
			flag = 0;
		}
		leave();
		sleep(1);
	}
}

int
closeChannel(){
	return OK;
}

void
enter(void)
{
	sem_wait(sd);
}

void
leave(void)
{
	sem_post(sd);
}

static sem_t *sd;

void
initmutex(void)
{
	if ( !(sd = sem_open(CLIENT_NAME, O_RDWR|O_CREAT, 0666, 1)) )
		fatal("sem_open");
}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}
