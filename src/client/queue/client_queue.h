#ifndef _CLIENT_QUEUE_H_ 
#define _CLIENT_QUEUE_H_ 

#include "../defs.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mqueue.h>

#define SERVER_NAME "/server_queue"

#define SIZE 1000

typedef struct
{
	long mtype;
	request_t mreq;
}msg_t;

/**
Retorna: si la operacion fue satifactoria o no
Abre la comunicacion con el servidor
*/
int openCChannel();

/**
Parametros: request
escribe el request en el fifo del servidor
*/
void sendRequest(request_t request);

/**
Retorna: el request que le manda el servidor
*/
request_t receiveRequest();

/**
Retorna: si la operacion fue satifactoria o no
cierra el canal de comunicacion entre el cliente
y el servidor
*/
int closeChannel();

void fatal(char *s);

#endif

