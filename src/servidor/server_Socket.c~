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

void compactaClaves (cIPC_t * ipcsClient, int *n);
int dameMaximo (cIPC_t * ipcsClient, int n);

cIPC_t ipcsClient[MAX_USERS];
fd_set descriptoresLectura; /* Descriptores de interes para select() */
int fdS;
int numeroClientes = 0; /* Número clientes conectados */

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
	
	/* Se inicializa descriptoresLectura */
	FD_ZERO (&descriptoresLectura);
	
	/* Se añade para select() el socket servidor */
	FD_SET (fdS, &descriptoresLectura);
	

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
	
	/* Se eliminan todos los clientes que hayan cerrado la conexión */
	compactaClaves (ipcsClient, &numeroClientes);
	
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
	
	/* Se añaden para select() el sockets con el cliente ya conectado */
	FD_SET (ipcsClient[numeroClientes].fd, &descriptoresLectura);		

	printf ("Aceptado cliente %d\n", clientPID);
	return OK;
	

	/*int rc, i;
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
	return OK;*/
}

request_t 
receiveRequest(){
	
	int maximo;
	int i, rc;
	request_t request;

	maximo = dameMaximo (ipcsClient, numeroClientes);
	
	if (maximo < fdS)
		maximo = fdS;
	
	/* Espera indefinida hasta que alguno de los descriptores tenga algo
	 * que decir: un mensaje de algun cliente ya conectado
	 * Parametros: 
	 * - el valor del descriptor más alto que queremos tratar más uno
	 * -  un puntero a los descriptores de los que nos interesa saber 
	 si hay algún dato disponible para leer o que queremos 
	 que se nos avise cuando lo haya
	 * - (ultimo parametro) struct timeval * es el tiempo que 
	 queremos esperar como máximo. Si pasamos NULL, nos quedaremos bloqueados 
	 en la llamada a select() hasta que suceda algo en alguno de 
	 los descriptores. Se puede poner un tiempo cero si únicamente 
	 queremos saber si hay algo en algún descriptor, sin quedarnos 
	 bloqueados.*/
	select(maximo + 1, &descriptoresLectura, NULL,NULL,NULL);
	
	/* Se comprueba si algún cliente ya conectado ha enviado algo */
	for (i=0; i<numeroClientes; i++)
	{
		/*nos indica si ha habido algo en el descriptor int dentro de fd_set*/
		if (FD_ISSET (ipcsClient[i].fd, &descriptoresLectura))
		{
			rc = recv(fdS, &request, sizeof(request), 0);
			if (rc < 0)
			{
				perror("recv() failed");
				//strncpy(request.name,"error",NAME_LENGTH);
				request.reqID = ERROR;
				return request;
			} 
			//printf("%d bytes of data were received\n", rc);
			printf("recibio request= %d\n",request.reqID);
			
			return request;
		}
	}
	
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
	/*char Spid[20];
	sprintf(Spid, "%d\0", pid);
	char clientPath2[20] = CLIENT_NAME;
	char * clientPath = strcat(clientPath2, Spid);
	remove(clientPath);*/
	return OK;
}

/*
 * Busca en array todas las posiciones con -1 y las elimina, copiando encima
 * las posiciones siguientes.
 * Ejemplo, si la entrada es (3, -1, 2, -1, 4) con *n=5
 * a la salida tendremos (3, 2, 4) con *n=3
 */
void 
compactaClaves (cIPC_t * ipcsClient, int *n)
{
	int i,j;
	
	if ((ipcsClient == NULL) || ((*n) == 0))
		return;
	
	j=0;
	for (i=0; i<(*n); i++)
	{
		if (ipcsClient[i].fd != -1)
		{
			ipcsClient[j] = ipcsClient[i];
			j++;
		}
	}
	
	*n = j;
}

/*
 * Función que devuelve el valor máximo en la tabla.
 * Supone que los valores válidos de la tabla son positivos y mayores que 0.
 * Devuelve 0 si n es 0 o la tabla es NULL */
int dameMaximo (cIPC_t * ipcsClient, int n)
{
	int i;
	int max;
	
	if ((ipcsClient == NULL) || (n<1))
		return 0;
	
	max = ipcsClient[0].fd;
	for (i=0; i<n; i++)
		if (ipcsClient[i].fd > max)
			max = ipcsClient[i].fd;
	
	return max;
}
