#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#define BUFSIZE 200

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

	char * server_name = "/tmp/fifosrv";
	char * client_name = "/tmp/fifocli";

	if ( access(server_name, 0) == -1 && mknod(server_name, S_IFIFO|0666, 0) == -1 )
	{
		fatal("(A) Error al abrir el fifo del servidor");
	}

	if ( access(client_name, 0) == -1 && mknod(client_name, S_IFIFO|0666, 0) == -1 )
	{
		fatal("(A) Error al abrir el fifo del cliente");
	}

	fdS = open(server_name, O_RDONLY);
	fdC = open(client_name, O_WRONLY);

	printf("Iniciando el servidor. \n");

	while(1){

		if(n = read(fdS, buf, BUFSIZE) > 0)
		{
			/* Aca recibo lo que manda el cliente y lo vuelvo a enviar. */

			printf("Servidor recibe: %.*s \n", BUFSIZE, buf);
			write(fdC, buf, BUFSIZE);
		}
	}
}
