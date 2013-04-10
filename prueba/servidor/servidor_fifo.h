#ifndef _SERVIDOR_FIFO_H_ 
#define _SERVIDOR_FIFO_H_ 

#include "../defs.h"

int createChannel();

void sendRequest(request_t);

request_t receiveRequest();

int closeChannel();
#endif
