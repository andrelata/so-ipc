#ifndef _SERVIDOR_FIFO_H_ 
#define _SERVIDOR_FIFO_H_ 

#include "../defs.h"
#include "servidor.h"

#define SERVER_NAME "/tmp/fifosrv"
#define CLIENT_NAME "/tmp/fifo"

/**
Abre el fijo del servidor
*/
int createServerChannel();

/**
Parametros: PID del cliente
Crea el canal de comunicacion con el cliente y abre el fifo del mismo.
*/
int createChannel(int clientPID);

/**
Paremetros: request
escribe en el fifo del cliente el request
*/
void sendRequest(request_t request);

/**
Retorna: request

*/
request_t receiveRequest();

/**
*/
int closeChannel();

/**
*/
int closeCChannel(int pid);

typedef struct cFifos {
	int pid;
	int fd;
} cFifos_t;
#endif
