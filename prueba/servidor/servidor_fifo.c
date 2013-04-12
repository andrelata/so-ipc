#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"
#include "./servidor_fifo.h"

int fdS, fdC;  

int 
createServerChannel(){
	if ( access(SERVER_NAME, 0) == -1 && mknod(SERVER_NAME, S_IFIFO|0666, 0) == -1 )
	{
		printf("(A) Error al abrir el fifo del servidor");
		return -1;
	}
	fdS = open(SERVER_NAME, O_RDONLY);
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

	write(fdC, &request, sizeof request);
}

int
createChannel(char * client_name){

	if ( access(client_name, 0) == -1 && mknod(client_name, S_IFIFO|0666, 0) == -1 )
	{
		printf("Error al abrir el fifo del cliente");
		return -1;
	}
	fdC = open(client_name, O_WRONLY);
	return 0;
}

int
closeChannel(){
	close(fdS);
	close(fdC);
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
