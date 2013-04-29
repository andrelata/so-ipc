#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../../defs.h"
#include "./servidor_queue.h"
#include "./servidorStruct.h"


char cltname[100];
msg_t msg;
char *msgptr = (char *) &msg;
mqd_t qin, qout;

int cltPID;


int 
createServerChannel(){
	
	
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof msg;
	printf("Abre canal del servidor\n");
		
	if ( (qin = mq_open(SERVER_NAME, O_RDONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal1("Error mq_open qin");

	return 0;

}

int
openNewChannel(int PID){

	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof msg;
	
	char serverPath[50];
	sprintf(serverPath, "/server_%d_queue", getpid());
	
	if ( (qin = mq_open(serverPath, O_RDONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal1("Error mq_open qin");

	request_t request;
	request.reqID = ANS;
	request.PID = PID;
	request.par1 = getpid();

	sendRequest(request);
	
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
	printf("Servidor recibe algo: %d\n", msg.mreq.reqID);
	if(request.PID != cltPID){
		char cltname2[50];
		mqd_t qout2;
		sprintf(cltname2, "/client_%d_queue", request.PID);
		if ( (qout2 = mq_open(cltname2, O_WRONLY)) == -1 )
			fatal1("Error mq_open qout");
		mq_send(qout2, msgptr, sizeof msg, 0);
	}else{
		mq_send(qout, msgptr, sizeof msg, 0);
	}
}

int
createChannel(int clientPID){
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof msg;
	cltPID = clientPID;
	sprintf(cltname, "/client_%d_queue", clientPID);
	if ( (qout = mq_open(cltname, O_WRONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal1("Error mq_open qout");
	return OK;
}

int
closeChannel(){
	return OK;
}

int
closeCChannel(int pid){
	mq_unlink(cltname);
	return OK;
}

void
fatal1(char *s)
{
	perror(s);
	exit(1);
}

void leave1(void);

void enter1(void);

