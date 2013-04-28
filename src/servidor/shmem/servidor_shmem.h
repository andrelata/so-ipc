#ifndef _SERVIDOR_FIFO_H_ 
#define _SERVIDOR_FIFO_H_ 

#include "../defs.h"
#include "servidor.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SERVER_NAME "/message"
#define CLIENT_NAME "/mutex"

#define SIZE 1000

/**
Retorna: si la operacion fue satifactoria o no
Abre el fijo del servidor
*/
int createServerChannel();

int openNewChannel(int PID);

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

void reqcpy(request_t *reqC, request_t *request);

void enter1(void);

void leave1(void);

void initmutex1(void);

int reqcmp(request_t * req1, request_t *req2);

void fatal1(char *s);

#endif
