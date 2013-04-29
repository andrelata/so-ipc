#ifndef _CLIENT_QUEUE_H_ 
#define _CLIENT_QUEUE_H_ 

#include "../defs.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mqueue.h>

#define SERVER_NAME "/server_queue"

#define SIZE 1000

typedef struct
{
	long mtype;
	request_t mreq;
}msg_t;

void fatal(char *s);

#endif

