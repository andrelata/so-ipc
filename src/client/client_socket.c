#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "../../defs.h"
#include "./client_IPC.h"

int fdS, fdC; 

void 
main(int argc, char *argv[])
{
	request_t request;
	do{	
		if(openCChannel()==-1)
			break;
		sendRequest(request);
		request = receiveRequest();		
	}while(0);
	closeChannel();	
}
int 
openSChannel()
{
	
}

int 
openCChannel()
{
	int rc;
	struct sockaddr_un serveraddr;

	fdS = socket(AF_UNIX, SOCK_STREAM, 0);
      	if (fdS < 0)
      	{
         	perror("socket() failed");
         	return ERROR;
      	}
	return OK;
	
	memset(&serveraddr, 0, sizeof(serveraddr));
      	serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, SERVER_NAME);
	
	rc = connect(fdS, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
      	if (rc < 0)
      	{
         	perror("connect() failed");
         	return -1;
      	}
	return 0;
}

void 
sendRequest(request_t request)
{
	int rc;
	char c;
	char buffer[BUFSIZE];

	c = getchar();	
	memset(buffer, c, 3);

      	rc = send(fdS, buffer, sizeof(buffer), 0);
      	if (rc < 0)
      	{
         	perror("send() failed");
      	}
}

request_t 
receiveRequest()
{
	int rc;
	char   buffer[BUFSIZE];
	request_t request;

	rc = recv(fdS, &buffer,
                   sizeof(buffer), 0);
        if (rc < 0)
        {
           	perror("recv() failed");
            	return request;
        }
	printf("Recibio del server:%s\n", buffer);
	
	return request;
}
int 
closeChannel()
{
        close(fdS);
	remove(SERVER_NAME);
}
