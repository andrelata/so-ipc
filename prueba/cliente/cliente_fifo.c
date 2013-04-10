#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <pthread.h>
#include "cliente_fifo.h"

#define FIFOS "/tmp/fifo"
#define BUFSIZE 200

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

void 
chau(int sig)
{
	printf("Padre recibe SIGPIPE y termina\n");
	exit(1);
}

void* read_xs(void * unused){

	char buffer[BUFSIZE];

	int pid, n, fd;
	char * client_name = "/tmp/fifocli";

	if ( access(client_name, 0) == -1 && mknod(client_name, S_IFIFO|0666, 0) == -1 )
	{
		fatal("(A) Error al abrir el fifo del servidor");
	}

	printf("(H) Abriendo canal de comunicacion del servidor.\n");

	fd = open(client_name, O_RDONLY);

	while ( (n = read(fd, buffer, BUFSIZE)) > 0 )
	{
		/* En este ciclo recibo lo que manda el servidor */
		printf("(H) El servidor mando: %.*s \n", n, buffer);
	}
	printf("(H) Termina el proceso hijo. Se deja de escuchar el servidor. \n");

	return NULL;

}

int
main(int argc, char **argv)
{
	
	int pid, n, fd;
	pthread_t thread_id;
	char buffer[BUFSIZE];

	char * server_name = "/tmp/fifosrv";

	/* Create a new thread. The new thread will run the print_xs
	function. */
	pthread_create (&thread_id, NULL, &read_xs, NULL);

	if ( access(server_name, 0) == -1 && mknod(server_name, S_IFIFO|0666, 0) == -1 )
	{
		fatal("(A) Error al abrir el fifo del cliente");
	}

	signal(SIGPIPE, chau);
	//sprintf(name, "/tmp/fifo%d", pid); /* El pid es el del hijo no el del padre.
	//				* Averiguar como se obtiene el de uno mismo */

	printf("(P) Abriendo canal de comunicacion del cliente.\n");

	fd = open(server_name, O_WRONLY);

	while ( (n = read(0, buffer, BUFSIZE)) > 0 )
	{
		printf("(P) Se ingreso la cadena: %.*s \n", n, buffer);
		write(fd, buffer, BUFSIZE);
	}

	printf("(P) Padre termina.\n");


}
	

//int
//main(int argc, char **argv)
//{
//	int pid, n, fd;	
//
//	char buffer[BUFSIZE];
//
//	char * server_name = "/tmp/fifosrv";
//	char * client_name = "/tmp/fifocli";
//
//	if ( access(server_name, 0) == -1 && mknod(server_name, S_IFIFO|0666, 0) == -1 )
//	{
//		fatal("(A) Error al abrir el fifo del servidor");
//	}
//
//	if ( access(client_name, 0) == -1 && mknod(client_name, S_IFIFO|0666, 0) == -1 )
//	{
//		fatal("(A) Error al abrir el fifo del cliente");
//	}
//
//	switch ( pid = fork() )
//	{
//		case -1:
//
//			fatal("Error al hacer fork");
//			break;
//
//		case 0:	/* hijo */
//
//			printf("(H) Abriendo canal de comunicacion del servidor.\n");
//
//			fd = open(server_name, O_RDONLY);
//
//			while ( (n = read(fd, buffer, BUFSIZE)) > 0 )
//			{
//				/* En este ciclo recibo lo que manda el servidor */
//				printf("(H) El servidor mando: %.*s \n", n, buffer);
//			}
//			printf("(H) Termina el proceso hijo. Se deja de escuchar el servidor. \n");
//			break;
//		
//		default: /* padre */
//
//			signal(SIGPIPE, chau);
//			//sprintf(name, "/tmp/fifo%d", pid); /* El pid es el del hijo no el del padre.
//			//				* Averiguar como se obtiene el de uno mismo */
//
//			printf("(P) Abriendo canal de comunicacion del cliente.\n");
//
//			fd = open(client_name, O_WRONLY);
//
//			while ( (n = read(0, buffer, BUFSIZE)) > 0 )
//			{
//				printf("(P) Se ingreso la cadena: %.*s \n", n, buffer);
//				write(fd, buffer, BUFSIZE);
//			}
//
//			printf("(P) Padre termina.\n");
//			break;
//	}
//}
