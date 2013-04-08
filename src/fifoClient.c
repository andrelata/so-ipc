#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include "fifoClient.h"

#define FIFOC "/tmp/.."


int 
main(int argc, char **argv)
{
	int pid;
	int fd;	
	char * buf[200];

	
}

bool
write(int fd){

	fd = open(FIFOC, O_WRONLY);
	
}

bool
read(int fd, char * buf[200]){
	
	fd = open(FIFOC, O_RDONLY);

}
