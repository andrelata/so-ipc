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
#include "./server_IPC.h"
#include "./servidorStruct.h"

#define MAX_USERS 20

cIPC_t ipcsClient[MAX_USERS];

int fdS;//, fdC;

/*void main()
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
}*/

int 
createServerChannel()
{
      	int rc;
	struct sockaddr_un serveraddr;

      	fdS = socket(AF_UNIX, SOCK_STREAM, 0);
      	if (fdS < 0)
      	{
         	perror("socket() failed");
        	return ERROR;
      	}
      
     	memset(&serveraddr, 0, sizeof(serveraddr));
     	serveraddr.sun_family = AF_UNIX;
      	strcpy(serveraddr.sun_path, SERVER_NAME);
	
      	rc = bind(fdS, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
      	if (rc < 0)
      	{
         	perror("bind() failed");
         	return ERROR;
      	}

	//inicializa los ipcsClient:

	int i;
	for(i=0;i<MAX_USERS;i++){
		ipcsClient[i].pid = EMPTY;
	}
      	return OK;
}

int 
createChannel(int clientPID){

	int rc, i;
	int * fdC;
	int flag = 0;

	for(i=0;i<MAX_USERS && flag == 0;i++){
		if(ipcsClient[i].pid == EMPTY){
			ipcsClient[i].pid = clientPID;
			fdC = &(ipcsClient[i].fd);
			flag = 1;
		}
	}

	if(flag == 0){
		return ERROR;
	}

	char pid[20];
	sprintf(pid, "%d\0", clientPID);
	printf("pid: %s\n", pid);
	char clientPath2[20] = CLIENT_NAME;
	char * clientPath = strcat(clientPath2, pid);
	printf("clientPath: %s\n", clientPath);	
	
	*fdC = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fdC < 0)
      	{
         	perror("socket() failed");
         	return ERROR;
      	}

	rc = listen(*fdC, 10);
      	if (rc< 0)
      	{
         	perror("listen() failed");
         	return ERROR;
      	}

      	printf("Ready for client connect().\n");

	*fdC = accept(*fdC, NULL, NULL);
      	if (fdC < 0)
      	{
         	perror("accept() failed");
         	return ERROR;
      	}
	printf("Abrio el socket %d\t%d\n", *fdC, ipcsClient[i-1].fd);
	return OK;
}

request_t 
receiveRequest(){

	int rc;
	//char buffer[BUFSIZE];
	request_t request;

      	rc = recv(fdS, &request, sizeof(request), 0);
      	if (rc < 0)
      	{
         	perror("recv() failed");
         	//strncpy(request.name,"error",NAME_LENGTH);
		return request;
      	} 
      	//printf("%d bytes of data were received\n", rc);
      	printf("recibio request= %d\n",request.reqID);
	
	//strncpy(request.name,"bb",NAME_LENGTH);

	return request;
}

void 
sendRequest(request_t request){
	int rc,i;
	int fdC;

	for(i=0;i<MAX_USERS;i++){
		if(ipcsClient[i].pid == request.PID){
			fdC = ipcsClient[i].fd;
		}
	}

	rc = send(fdC, &request, sizeof(request), 0);
     	if (rc < 0)
      	{
         	perror("send() failed");
         	return;
      	}
	printf("escribio en el fifo %d del client: %d\n", fdC, request.PID);
}

//repite codigo, con servidor_fifo.c
//estaria bueno hacer un .c con estas funciones, y hacer un .h 
//que lo incluya en los 2 archivos, y que tenga las variables
//ese c. variables externas.

int 
closeChannel(){
	
	int i;
	
 	close(fdS);
      	
	for(i=0;i<MAX_USERS;i++){
		if(ipcsClient[i].pid != EMPTY){
			close(ipcsClient[i].fd);
		}
	}
	remove(SERVER_NAME);
	printf("cierro puertos\n");
	return OK;

}

int
closeCChannel(int pid){
	int i;
	for(i=0;i<MAX_USERS;i++){
		if(ipcsClient[i].pid == pid){
			printf("cerrando canal: %d\n", ipcsClient[i].fd);
			close(ipcsClient[i].fd);
			ipcsClient[i].pid = EMPTY;
			ipcsClient[i].fd = 0;
		}
	}
	char Spid[20];
	sprintf(Spid, "%d\0", pid);
	char clientPath2[20] = CLIENT_NAME;
	char * clientPath = strcat(clientPath2, Spid);
	remove(clientPath);
	return OK;
}