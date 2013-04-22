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
request_t *reqS;
request_t *reqC;
request_t buf;
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

	return 0;
}

int openCChannel(){
	/*char pid[20];
	sprintf(pid, "%d\0", getpid());
	char clientPath2[20] = CLIENT_NAME;
	char * clientPath = strcat(clientPath2, pid);
	printf("clientPath: %s %s\n", clientPath, clientPath2);
	if( (fdC = open(clientPath, O_RDONLY)) < 0 ){
		return ERROR;
	}
	printf("abrio el canal %d\n", fdC);
	return OK;*/
}

request_t 
receiveRequest(){
	int flag = 1;
	request_t request;

	while ( flag )
	{
		enter();
		if ( reqcmp(&buf, reqS) )
		{
			printf("Servidor recibe: %d", reqS->reqID);
			reqcpy(&buf, reqS);
			flag = 0;
		}
		leave();
		sleep(1);
	}
	
	reqcpy(&request, reqS);
	return request;
}


void 
sendRequest(request_t request){
	
	printf("le envio al client: %d\n", request.PID);

	enter();
	reqcpy(reqC, &request);
	leave();
}

int
closeChannel(){
	/*close(fdS);
	close(fdC);*/
}

void
reqcpy(request_t *reqC, request_t *request){
	reqC->reqID = request->reqID;
	reqC->PID = request->PID;
	reqC->par1 = request->par1;
	strcpy(reqC->name,request->name);
	strcpy(reqC->message,request->message);
	reqC->time = request->time;
	reqC->price = reqC->price;
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

int
reqcmp(request_t * req1, request_t *req2){
	if(req1->reqID == req2->reqID && req1->PID == req2->PID && req1->par1 == req2->par1 && strcmp(req1->name,req2->name) && strcmp(req1->message,req2->message) && req1->time == req2->time && req1->price == req2->price ){
		return 0;
	}
	return 1;
}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}
