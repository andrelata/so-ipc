#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../../defs.h"
#include "./client_queue.h"

char cltname[100];
msg_t msg;
char *msgptr = (char *) &msg;
mqd_t qin, qout;

int openSChannel(){
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof msg;

	printf("Abre canal del servidor\n");

	if ( (qout = mq_open(SERVER_NAME, O_WRONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal("Error mq_open qout");

	return 0;
}

int openCChannel(){

	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof msg;

	sprintf(cltname, "/client_%d_queue", getpid());
	if ( (qin = mq_open(cltname, O_RDONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal("Error mq_open qin");	

	request_t req;
	req = receiveRequest();
	if(req.reqID != ANS){
		printf("error\n");
		return ERROR;
	}

	char serverPath[50];
	sprintf(serverPath, "/server_%d_queue", req.par1);
	
	if ( (qout = mq_open(serverPath, O_WRONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal("Error mq_open qout");
	return OK;
}

request_t 
receiveRequest(){
	int n;
	msg_t msg;
	char *msgptr = (char *) &msg;
	int offset = (char *) &(msg.mreq) - msgptr;
	while( !( (n = mq_receive(qin, msgptr, sizeof msg, NULL)) > 0 ))
	{		
	}
		printf("Servidor recibe algo: %d\n", msg.mreq.reqID);
	
	return msg.mreq;
}

void 
sendRequest(request_t request){
	msg;
	char *msgptr = (char *) &msg;
	int offset = (char *) &(msg.mreq) - msgptr;
	memcpy(&(msg.mreq), &request, sizeof request);
	printf("Servidor envia algo: %d\n", msg.mreq.reqID);
	
	mq_send(qout, msgptr, sizeof msg, 0);
}

int
closeChannel(){
	return OK;
}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}
