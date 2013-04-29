#ifndef _SERVIDOR_FIFO_H_ 
#define _SERVIDOR_FIFO_H_ 

#include "../defs.h"
#include "servidorC.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SERVER_NAME "/message"
#define CLIENT_NAME "/mutex"

#define SIZE 1000

void reqcpy(request_t *reqC, request_t *request);

void enter1(void);

void leave1(void);

void initmutex1(void);

int reqcmp(request_t * req1, request_t *req2);

void fatal1(char *s);

#endif
