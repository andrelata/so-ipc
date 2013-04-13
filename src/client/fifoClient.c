#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include "fifoClient.h"

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

int
main(int argc, char **argv)
{
	int pid, n;
	int fd;	
	char * buf[BUFSIZE];
	char name[20];

	switch ( pid = fork() )
	{
		case -1:
			fatal("Error fork");
			break;

		case 0: /* hijo */
			fd = open(FIFOS, O_RDONLY);
			while ( (n = read(fd, buf, BUFSIZE)) > 0 )
			{
				/*aca recibo lo que manda el servidor*/
				printf("Hijo lee del pipe: %.*s", n, buf);
				if ( memcmp(buf, "end", 3) == 0 )
					break;
			}
			printf("Hijo termina\n");
			break;
		
		default: /* padre */
			signal(SIGPIPE, chau);
			sprintf(name, "/tmp/fifoC");  /*%d", pid); /*el pid es el del hijo no el del padre. 
								Averiguar como se obtiene el de uno mismo */
			fd = mkfifo(name, S_IFIFO|0666);
			while ( (n = read(0, buf, BUFSIZE)) > 0 )
			{
				printf("Padre escribe en el pipe: %.*s", n, buf);
				write(fd, buf, n);
			}
			printf("Padre termina\n");
			break;
	}
}
