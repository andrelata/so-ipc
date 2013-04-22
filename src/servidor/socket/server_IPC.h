#ifndef _SERVIDOR_IPC_H_ 
#define _SERVIDOR_IPC_H_ 

#include "../../defs.h"
//#include "servidor.h"

#define SERVER_NAME "/tmp/server"

/**
Retorna: si la operacion fue satifactoria o no
Abre el fijo del servidor
*/
int createServerChannel();

/**
Parametros: PID del cliente
Retorna: si la operacion fue satifactoria o no
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
recibe el request que le manda el cliente y lo retorna
*/
request_t receiveRequest();

/**
Retorna: si la operacion fue satifactoria o no
cierra la comunicacion con todos los fifos del cliente
*/
int closeChannel();

/**
Parametros: Pid del usuario
Retorna: si la operacion fue satifactoria o no
cierra la comunicacion con el fifo especificado
*/
int closeCChannel(int pid);

typedef struct cIPC {
	int pid;
	int fd;
} cIPC_t;
#endif
