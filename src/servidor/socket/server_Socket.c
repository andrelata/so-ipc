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
#include "./server_IPC.h"
//#include "./servidorStruct.h"

#define MAX_USERS 20

cIPC_t ipcsClient[MAX_USERS];
int fdS, fdC;

void main()
{	
	request_t request;
	do{
		if(createServerChannel() == -1)
			break;
		if(createChannel(0) == -1)
			break;
		request = receiveRequest();
		sendRequest(request);
	}while(0);
	closeChannel();
}

/**
Retorna: si la operacion fue satifactoria o no
Abre el fijo del servidor
*/
int 
createServerChannel()
{
      	int rc;
	struct sockaddr_un serveraddr;

      	fdS = socket(AF_UNIX, SOCK_STREAM, 0);
      	if (fdS < 0)
      	{
         	perror("socket() failed");
        	return -1;
      	}
      
     	memset(&serveraddr, 0, sizeof(serveraddr));
     	serveraddr.sun_family = AF_UNIX;
      	strcpy(serveraddr.sun_path, SERVER_NAME);
	
      	rc = bind(fdS, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
      	if (rc < 0)
      	{
         	perror("bind() failed");
         	return -1;
      	}

	//inicializa los ipcsClient:

	/*int i;
	for(i=0;i<MAX_USERS;i++){
		ipcsClient[i].pid = EMPTY;
	}
      	return 0;*/
}

int 
createChannel(int clientPID){

	int rc;

	rc = listen(fdS, 10);
      	if (rc< 0)
      	{
         	perror("listen() failed");
         	return -1;
      	}

      	printf("Ready for client connect().\n");

	fdC = accept(fdS, NULL, NULL);
      	if (fdC < 0)
      	{
         	perror("accept() failed");
         	return -1;
      	}
	return 0;
}

request_t 
receiveRequest(){

	int rc;
	char buffer[BUFSIZE];
	request_t request;

      	rc = recv(fdC, buffer, sizeof(buffer), 0);
      	if (rc < 0)
      	{
         	perror("recv() failed");
         	strncpy(request.name,"error",NAME_LENGTH);
		return request;
      	} 
      	printf("%d bytes of data were received\n", rc);
      	printf("buffer= %s\n",buffer);
	
	strncpy(request.name,buffer,NAME_LENGTH);

	return request;
}

void 
sendRequest(request_t request){
	int rc;

	rc = send(fdC, request.name, sizeof(NAME_LENGTH), 0);
     	if (rc < 0)
      	{
         	perror("send() failed");
         	return;
      	}
}

int 
closeChannel(){
	if (fdS != -1)
      		close(fdS);

   	if (fdC != -1)
      		close(fdC);
	return 0;
}
