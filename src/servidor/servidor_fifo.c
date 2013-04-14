#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"
#include "./servidor_fifo.h"
#include "./servidorStruct.h"

int fdS;

cFifos_t fifos[MAX_USERS];

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
		fifos[i].pid = -1;
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

//GONZA: no entiendo porque aca le mandas el ClientePID
//cuando le podes mandar el PID por el request?

//ANDRE: tenes razon, corregido!
void 
sendRequest(request_t request){

	int fdC;
	int i;

	printf("request.pid: %d, fdC: %d\n", request.PID, fdC);

	for(i=0;i<MAX_USERS;i++){
		printf("userID: %d, fd: %d\n", fifos[i].pid, fifos[i].fd);
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
		if(fifos[i].pid == -1){
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
		if(fifos[i].pid != -1){
			close(fifos[i].fd);
		}
	}
}

int
closeCChannel(int pid){
	int i;
	for(i=0;i<MAX_USERS;i++){
		if(fifos[i].pid == pid){
			printf("cerrando canal: %d\n", fifos[i].fd);
			close(fifos[i].fd);
			fifos[i].pid = -1;
			fifos[i].fd = 0;
		}
	}
	return OK;
}

/*
void
fatal(char *s)
{
	perror(s);
	exit(1);
}

int
main(int argc, char **argv)
{
	//int pid, n;
	char buf[BUFSIZE];

	//int fdS;
	//int fdC;

	char * client_name = "/tmp/fifocli";

	request_t request;

	if ( access(SERVER_NAME, 0) == -1 && mknod(SERVER_NAME, S_IFIFO|0666, 0) == -1 )
	{
		fatal("(A) Error al abrir el fifo del servidor");
	}

	/*if ( access(client_name, 0) == -1 && mknod(client_name, S_IFIFO|0666, 0) == -1 )
	{
		fatal("(A) Error al abrir el fifo del cliente");
	}
	
	int fdC = createChannel(client_name);

	fdS = open(SERVER_NAME, O_RDONLY);
	//fdC = open(client_name, O_WRONLY);

	printf("Iniciando el servidor. \n");

	while(1){

		request = receiveRequest();
		sendRequest(request, fdC);
	}
}*/