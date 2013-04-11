#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"

#define SERVER_NAME "/tmp/fifosrv"

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

int
main(int argc, char **argv)
{
	int pid, n;
	char buf[BUFSIZE];

	int fdS, fdC;

	char * client_name = "/tmp/fifocli";

	request_t request;

	if ( access(SERVER_NAME, 0) == -1 && mknod(SERVER_NAME, S_IFIFO|0666, 0) == -1 )
	{
		fatal("(A) Error al abrir el fifo del servidor");
	}

	if ( access(client_name, 0) == -1 && mknod(client_name, S_IFIFO|0666, 0) == -1 )
	{
		fatal("(A) Error al abrir el fifo del cliente");
	}

	fdS = open(SERVER_NAME, O_RDONLY);
	fdC = open(client_name, O_WRONLY);

	printf("Iniciando el servidor. \n");

	while(1){

		if(n = read(fdS, &request, sizeof request ) > 0)
		{
			/* Aca recibo lo que manda el cliente y lo vuelvo a enviar. */

			printf("Servidor recibe: %c \n", request.reqID);
			write(fdC, &request, sizeof request);
		}
	}
}


int
createChannel(){

	
}