#ifndef _CLIENT_FIFO_H_ 
#define _CLIENT_FIFO_H_ 

#include "../defs.h"

#define SERVER_NAME "/tmp/fifosrv"
#define CLIENT_NAME "/tmp/fifo"

int createServerChannel();

int createChannel(char * client_name);

void sendRequest(request_t request);

request_t receiveRequest();

int closeChannel();
#endif
