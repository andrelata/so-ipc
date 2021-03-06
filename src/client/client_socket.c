#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "../defs.h"
#include "./cliente_IPC.h"

int fdS, fdC;
char serverPath[50];


int 
openCChannel()
{
	int rc;
	char clientPath[20];
	struct sockaddr_un clientaddr;
	
	sprintf(clientPath, "%s%d", CLIENT_NAME, getpid());
	printf("El path del cliente es: %s\n", clientPath);

	/* se abre el descriptor del server */

	fdC = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (fdC < 0)
	{
		perror("socket() failed");
		return ERROR;
	}

	memset(&clientaddr, 0, sizeof(clientaddr));
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path, clientPath);
	
	rc = bind(fdC, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
	if (rc < 0)
	{
		perror("bind() failed");
		return ERROR;
	}

	return OK;
}

int 
openSChannel()
{
	int rc;
	struct sockaddr_un serveraddr;

	/* se abre el descriptor del server */

	fdS = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (fdS < 0)
	{
		perror("socket() failed");
		return ERROR;
	}

	strcpy(serverPath, SERVER_NAME);

	return OK;
}

void 
sendRequest(request_t request)
{
	int rc;
	struct sockaddr_un serveraddr;
	
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, serverPath);
	
	rc = sendto(fdS, &request, sizeof(request), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (rc < 0)
	{
		perror("sendto() failed");
	}
}

request_t 
receiveRequest()
{
	int rc;
	struct sockaddr_un serveraddr;
	int serveraddrlen = sizeof(serveraddr);

	request_t request;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, CLIENT_NAME);

	rc = recvfrom(fdC, &request, sizeof(request), 0, (struct sockaddr *)&serveraddr, & serveraddrlen);

	if (rc < 0)
	{
		perror("recv() failed");
		request.reqID = ERROR;
		return request;
	}
	printf("Recibio del server:%d\n", request.reqID);

	if(request.reqID == ANS){
		sprintf(serverPath, "/server_%d_queue", request.par1);
	}

	return request;
}

int 
closeChannel()
{
	close(fdS);
	remove(SERVER_NAME);
}
