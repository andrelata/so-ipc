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
#include "./servidor_IPC.h"
#include "./servidorStruct.h"

#define SERVER_NAME "/tmp/server"

cIPC_t ipcsClient[MAX_USERS];
int fdS;
int numeroClientes = 0;

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
	/*
	 * Avisamos al sistema que comience a atender peticiones de clientes.
	 */
	if (listen (fdS, 1) == -1)
	{
		close (fdS);
		return ERROR;
	}

	//inicializa los ipcsClient:

	int i;
	for(i=0;i<MAX_USERS;i++){
		ipcsClient[i].pid = EMPTY;
		ipcsClient[i].fd = -1;
		
	}
	return OK;
}

int 
createChannel(int clientPID){
	
	socklen_t Longitud_Cliente;
	struct sockaddr Cliente;
	int fdC;

	Longitud_Cliente = sizeof(Cliente);
	fdC = accept(fdS, &Cliente, &Longitud_Cliente);
	if(fdC == -1)
		return ERROR;
	

	if ((numeroClientes) <= MAX_USERS)
	{		
		ipcsClient[numeroClientes].pid = clientPID;
		ipcsClient[numeroClientes].fd = fdC;
		numeroClientes++;
	}
	else {
		return ERROR;
	}
		
	printf ("Aceptado cliente %d\n", clientPID);
	return OK;
	
}

request_t 
receiveRequest(){
	
	int maximo;
	int i, rc;
	request_t request;

	//rc = recv(fdS, &request, sizeof(request), 0);
	rc = read (fdS, &request, sizeof(request));
	while (rc < 0)
	{
		//perror("recv() failed");

		request.reqID = ERROR;
		//return request;
	} 

	printf("recibio request= %d\n",request.reqID);
			
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

	//rc = send(fdC, &request, sizeof(request), 0);
	rc = write (fdC,  &request, sizeof(request));
     	if (rc < 0)
      	{
         	perror("send() failed");
         	return;
      	}
	printf("escribio en el client: %d\n", request.PID);
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
			ipcsClient[i].fd = -1;
		}
	}

	return OK;
}

