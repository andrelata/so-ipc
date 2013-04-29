#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"
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
		fatal("sh_open");
	ftruncate(fdS, SIZE);
	if ( !(reqS = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fdS, 0)) )
		fatal("mmap");
	close(fdS);
	reqC = reqS + 1;
	initmutex();
	reqS->reqID = EMPTY;
	reqC->reqID = EMPTY;

	return 0;

}

int
openNewChannel(int PID){
	
	/*char pid[20];
	sprintf(pid, "%d\0", getpid());
	char serverPath2[20] = SERVER_NAME;
	char * serverPath = strcat(serverPath2, pid);
	
	if ( (fdS = shm_open(serverPath, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fdS, SIZE);
	if ( !(reqS = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fdS, 0)) )
		fatal("mmap");
	close(fdS);

	request_t request;
	request.reqID = ANS;
	request.PID = PID;
	memcpy(&request.message[0], &reqS, sizeof reqS);

	sendRequest(request);

	reqC = reqS + 1;
	initmutex();
	reqS->reqID = EMPTY;
	reqC->reqID = EMPTY;*/

	
	
	return OK;
}

request_t 
receiveRequest(){
	int flag = 1;
	request_t request;

	printf("leyendo desde: %p\n", reqS);

	while ( flag )
	{
		enter();
		if ( reqS->reqID != EMPTY )
		{
			printf("Servidor recibe: %d", reqS->reqID);
			memcpy(&request, reqS, sizeof request);
			reqS->reqID = EMPTY;
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
	printf("le envio al client: %d req: %d\n", request.PID, request.reqID);

	while(flag){
		enter();
		if(reqC->reqID == EMPTY ){
			reqC->price = request.price;
			memcpy(reqC, &request, sizeof request);
			flag = 0;
		}
		leave();
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
enter(void)
{
	sem_wait(sd);
}

void
leave(void)
{
	sem_post(sd);
}


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

