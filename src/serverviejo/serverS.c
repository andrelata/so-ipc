#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#define FIFOS "/tmp/fifo"
#define BUFSIZE 200

int
main(int argc, char **argv)
{
	int pid, n;
	int fdS, fdC;	
	char buf[BUFSIZE];
	char name[20];

	fdS = mkfifo(FIFOS, S_IFIFO|0666);
	fdC = open("/tmp/fifoC", O_RDONLY);
	while(1){
		if(n = read(fdC, buf, BUFSIZE) > 0)
		{
			/*aca recibo lo que manda el cliente*/
			printf("Servidor recibe: %.*s", n, buf);
			write(fdS, buf, n);
		}
			
	}
}
