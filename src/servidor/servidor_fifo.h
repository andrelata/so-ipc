#ifndef _SERVIDOR_FIFO_H_ 
#define _SERVIDOR_FIFO_H_ 

#include "../defs.h"
#include "servidor.h"

#define SERVER_NAME "/tmp/fifosrv"
#define CLIENT_NAME "/tmp/fifo"

int createServerChannel();

int createChannel(int clientPID);

void sendRequest(request_t request);

request_t receiveRequest();

int closeChannel();

int closeCChannel(int pid);

typedef struct cFifos {
	int pid;
	int fd;
} cFifos_t;
#endif
