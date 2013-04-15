#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"
#include "./client_fifo.h"

int fdS, fdC; 

int openSChannel(){
	if( (fdS = open(SERVER_NAME, O_WRONLY)) < 0)
	{
		return ERROR;
	}
	return OK;
}

int openCChannel(){
	char pid[20];
	sprintf(pid, "%d\0", getpid());
	char clientPath2[20] = CLIENT_NAME;
	char * clientPath = strcat(clientPath2, pid);
	printf("clientPath: %s %s\n", clientPath, clientPath2);
	if( (fdC = open(clientPath, O_RDONLY)) < 0 ){
		return ERROR;
	}
	printf("abrio el canal %d\n", fdC);
	return OK;
}

request_t 
receiveRequest(){
	int n;
	request_t request;

	if(n = read(fdC, &request, sizeof request ) > 0)
	{
		/* Aca recibo lo que manda el cliente */
		//printf("Servidor recibe: %d\n", request.reqID);
		return request;
	}
}


void 
sendRequest(request_t request){
	
	write(fdS, &request, sizeof request);
}

int
closeChannel(){
	close(fdS);
	close(fdC);
}
