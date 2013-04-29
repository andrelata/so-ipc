#ifndef _SERVIDOR_QUEUE_H_ 
#define _SERVIDOR_QUEUE_H_ 

#include "../defs.h"
#include "servidorC.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mqueue.h>

#define SERVER_NAME "/server_queue"

typedef struct
{
	long mtype;
	request_t mreq;
}msg_t;

void fatal1(char *s);

#endif
