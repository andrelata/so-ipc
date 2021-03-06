#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"
#include "./servidor_IPC.h"
#include "./servidorStruct.h"

int fdS;

cIPC_t fifos[MAX_USERS];

int 
createServerChannel(){
	if ( access(SERVER_NAME, 0) == -1 && mknod(SERVER_NAME, S_IFIFO|0666, 0) == -1 )
	{
		printf("(A) Error al abrir el fifo del servidor");
		return -1;
	}
	fdS = open(SERVER_NAME, O_RDONLY);

	//inicializa los cFifos:

	int i;
	for(i=0;i<MAX_USERS;i++){
		fifos[i].pid = EMPTY;
	}
	return 0;
}

request_t 
receiveRequest(){
	int n;
	request_t request;

	if(n = read(fdS, &request, sizeof request ) > 0)
	{
		/* Aca recibo lo que manda el cliente */
		printf("Servidor recibe: %d\n", request.reqID);
		return request;
	}
}

void 
sendRequest(request_t request){

	int fdC;
	int i;

	for(i=0;i<MAX_USERS;i++){
		if(fifos[i].pid == request.PID){
			fdC = fifos[i].fd;
		}
	}

	printf("escribio en el fifo %d del client: %d\n", fdC, request.PID);

	write(fdC, &request, sizeof request);
}

int
createChannel(int clientPID){

	int * fdC;
	int flag = 0;
	int i;

	for(i=0;i<MAX_USERS && flag == 0;i++){
		if(fifos[i].pid == EMPTY){
			fifos[i].pid = clientPID;
			fdC = &(fifos[i].fd);
			flag = 1;
		}
	}

	if(flag == 0){
		return ERROR;
	}

	char pid[20];
	sprintf(pid, "%d\0", clientPID);
	printf("pid: %s\n", pid);
	char clientPath2[20] = CLIENT_NAME;
	char * clientPath = strcat(clientPath2, pid);
	printf("clientPath: %s\n", clientPath);

	if ( access(clientPath, 0) == -1 && mknod(clientPath, S_IFIFO|0666, 0) == -1 )
	{
		printf("Error al abrir el fifo del cliente");
		return -1;
	}
	*fdC = open(clientPath, O_WRONLY);
	printf("Abrio el fifo %d\t%d\n", *fdC, fifos[i-1].fd);
	return 0;
}

int
closeChannel(){
	close(fdS);
	int i;
	for(i=0;i<MAX_USERS;i++){
		if(fifos[i].pid != EMPTY){
			close(fifos[i].fd);
		}
	}
	remove(SERVER_NAME);
}

int
closeCChannel(int pid){
	int i;
	for(i=0;i<MAX_USERS;i++){
		if(fifos[i].pid == pid){
			printf("cerrando canal: %d\n", fifos[i].fd);
			close(fifos[i].fd);
			fifos[i].pid = EMPTY;
			fifos[i].fd = 0;
		}
	}
	char Spid[20];
	sprintf(Spid, "%d\0", pid);
	char clientPath2[20] = CLIENT_NAME;
	char * clientPath = strcat(clientPath2, Spid);
	remove(clientPath);
	return OK;
}

