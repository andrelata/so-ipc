#ifndef _CLIENTE_FIFO_H_ 
#define _CLIENTE_FIFO_H_ 

#include "../defs.h"

int openChannel();

void sendRequest(request_t);

request_t receiveRequest();

int closeChannel();

#endif
