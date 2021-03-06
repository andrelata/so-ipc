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

int 
openCChannel()
{
	return OK;
}

int 
openSChannel()
{
	int rc;
	struct sockaddr_un serveraddr;

	/*se abre el descriptor del server*/
	fdS = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fdS < 0)
	{
		perror("socket() failed");
		return ERROR;
	}
	return OK;
	
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, SERVER_NAME);
	
	fdC = connect(fdS, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
	if (rc < 0)
	{
		perror("connect() failed");
		return 	ERROR;
	}
	prtinf("Conexion establecida");
	return OK;
}

void 
sendRequest(request_t request)
{
	int rc;
	rc = send(fdS, &request, sizeof(request), 0);
	//rc = write (fdS,  &request, sizeof(request));
	if (rc < 0)
	{
		perror("send() failed");
	}
}

request_t 
receiveRequest()
{
	int rc;
	request_t request;

	rc = recv(fdC, &request, sizeof(request), 0);
	//rc = read (fdC, &request, sizeof(request));
	if (rc < 0)
	{
		perror("recv() failed");
		request.reqID = ERROR;
		return request;
	}
	printf("Recibio del server:%d\n", request.reqID);
	return request;
}
int 
closeChannel()
{
	close(fdS);
	remove(SERVER_NAME);
}
