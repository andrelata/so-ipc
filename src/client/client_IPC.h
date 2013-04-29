#ifndef _CLIENT_FIFO_H_ 
#define _CLIENT_FIFO_H_ 

#include "../defs.h"

/**
Retorna: si la operacion fue satifactoria o no
Abre el canal de comunicacion entre el cliente y el servidor
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

/**
Retorna: si la operacion fue satifactoria o no
Abre la comunicacion con el servidor
*/
int openSChannel();

#endif

