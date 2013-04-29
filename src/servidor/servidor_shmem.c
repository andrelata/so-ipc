#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"
#include "./servidor_IPC.h"
#include "./servidor_shmem.h"
#include "./servidorStruct.h"

int fdS;
request_t *reqS;
request_t *reqC;
request_t buf;
static sem_t *sd;

int 
createServerChannel(){
	
	
	if ( (fdS = shm_open(SERVER_NAME, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal1("sh_open");
	ftruncate(fdS, SIZE);
	if ( !(reqS = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fdS, 0)) )
		fatal1("mmap");
	close(fdS);
	reqC = reqS + 1;
	initmutex1();
	reqS->reqID = EMPTY;
	reqC->reqID = EMPTY;

	return 0;

}

int
openNewChannel(int PID){
	
	char pid[20];
	sprintf(pid, "%d\0", getpid());
	char serverPath2[20] = SERVER_NAME;
	char * serverPath = strcat(serverPath2, pid);
	
	if ( (fdS = shm_open(serverPath, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal1("sh_open");
	ftruncate(fdS, SIZE);
	if ( !(reqS = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fdS, 0)) )
		fatal1("mmap");
	close(fdS);

	request_t request;
	request.reqID = ANS;
	request.PID = PID;
	request.par1 = getpid();

	sendRequest(request);

	//reqC = reqS + 1;
	initmutex1();
	reqS->reqID = EMPTY;
	reqC->reqID = EMPTY;

	
	
	return OK;
}

request_t 
receiveRequest(){
	int flag = 1;
	request_t request;

	printf("leyendo desde: %p\n", reqS);

	while ( flag )
	{
		enter1();
		if ( reqS->reqID != EMPTY )
		{
			printf("Servidor recibe: %d\n", reqS->reqID);
			memcpy(&request, reqS, sizeof request);
			reqS->reqID = EMPTY;
			flag = 0;
		}
		leave1();
		sleep(1);
	}
	
	return request;
}

void 
sendRequest(request_t request){
	int flag = 1;
	printf("le envio al client: %d req: %d\n", request.PID, request.reqID);

	while(flag){
		enter1();
		if(reqC->reqID == EMPTY ){
			reqC->price = request.price;
			memcpy(reqC, &request, sizeof request);
			flag = 0;
		}
		leave1();
		sleep(1);
	}
}

int
createChannel(int clientPID){

	return OK;
}

int
closeChannel(){
	return OK;
}

int
closeCChannel(int pid){

	return OK;
}

void
enter1(void)
{
	sem_wait(sd);
}

void
leave1(void)
{
	sem_post(sd);
}


void
initmutex1(void)
{
	char pid[20];
	sprintf(pid, "%d\0", getpid());
	char serverPath2[20] = SERVER_NAME;
	char * serverPath = strcat(serverPath2, pid);
	if ( !(sd = sem_open(serverPath, O_RDWR|O_CREAT, 0666, 1)) )
		fatal1("sem_open");
}

void
fatal1(char *s)
{
	perror(s);
	exit(1);
}

void leave1(void);

void enter1(void);

