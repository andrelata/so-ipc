#ifndef _CLIENT_FIFO_H_ 
#define _CLIENT_FIFO_H_ 

#include "../defs.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SERVER_NAME "/tmp/shmemsrv"
#define CLIENT_NAME "/tmp/shmem"

#define SIZE 1000

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

void reqcpy(request_t *reqC, request_t *request);

void enter(void);

void leave(void);

void initmutex(void);

int reqcmp(request_t * req1, request_t *req2);

void fatal(char *s);

#endif
