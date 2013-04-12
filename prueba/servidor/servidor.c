#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "../defs.h"
#include "./servidor_fifo.h"
#include "./servidor.h"
#include "./servidorStruct.h"

session_t session[MAXSESSION];
user_t user;

int
main(int argc, char **argv)
{
	char buf[BUFSIZE];
	char * client_name = "/tmp/fifocli";
	request_t request;
	/*session_t session;
	user_t user;*/

	if(createChannel(client_name) == -1)
		return 0;
	if(createServerChannel() == -1)
		return 0;

	printf("Iniciando el servidor. \n");

	while(1){
		request = receiveRequest();
		parserRequest(request);
		if(request.reqID != 1)
			sendRequest(request);
	}
}

void
parserRequest(request_t request){
	
	switch(request.reqID){
		case 1: 
			//connect
			//parametros userID_t(PID), char nick[20]
			connectClient(request.PID, request.name, &user);
			break;
		case 2:
			//disconnect
			//parametro PID
			disconnect(request.PID);
			break;
		case 3:
			//getSession
			//parametro PID
			getSession(request.PID);
			break;
		case 4:
			//joinSession
			//parametro PID, int sessionID
			joinSession(request.PID, request.par1, &user);
			break;

		case 5:
			//exitSession
			//parametro PID
			exitSession(request.PID);
			break;
		case 6:
			//createSession
			//parametro PID, char name[20]
			createSession(request.PID, request.name);
			break;
		case 7: 
			//sentText 
			//parametros PID, char text[50]
			sentText(request.PID, request.message);
			break;
		case 8:
			//checkPrice
			//parametros PID
			checkPrice(request.PID);
			break;
		case 9:
			//changeState
			//parametros PID, int state
			changeState(request.PID, request.par1);
			break;
		case 0:
			//checkSession
			//parametro PID, user
			checkSession(request.PID, &user);
			break;
	}
}

void 
connectClient(pid_t PID, char * name, user_t * user){

	/*aca deberia ir la conexion con el cliente*/

	strcpy(user->nickname, name);
	user->userID = PID;
	user->state = 1; /*conectado = 1*/
	user->sessionID = -1; /*no tiene session asignada = -1*/
	user->time = time(NULL);

	int i;
	for(i = 0; i < MAXSESSION; i++)
	{
		session[i].id = -1; /*indica que la session esta vacia*/
	}	
	
	request_t request;
	request.reqID = 3;
	request.par1 = 0;
	
	sendRequest(request);	
}

void 
disconnect(pid_t PID){
	
}

void getSession(pid_t PID)
{
}

void 
joinSession(pid_t PID, int n, user_t * user){
	
	request_t request;
	int i;
	
	request.reqID = 3;
	for(i = 0; i < MAXSESSION; i++)
	{
		if(session[i].id == n)
		{
			user->sessionID = n;
			user->state = 2; /*en sesion*/
			session[i].users += 1; 
			request.par1 = 0;
			break;
		}
	}
	if(i == MAXSESSION)
		request.par1 = 1;
	sendRequest(request);
	
}

void 
exitSession(pid_t PID){
}

void 
createSession(pid_t PID, char * name){
	request_t request;
	int i;

	request.reqID = 3;

	for(i = 0; i < MAXSESSION; i++)
	{
		if(strcmp(session[i].name, name) == 0){
			request.par1 = 1;
			break;
		}
		if(session[i].id == -1)
			break;	
	}
	if(request.par1 != 1 && i < MAXSESSION)
	{	
		session[i].id = i;
		strcpy(session[i].name,name);
		session[i].users = 1;
		request.par1 = 0;
	}
	
	sendRequest(request);
	
}



void 
sentText(pid_t PID, char * message){
}

void 
checkPrice(pid_t PID){
}

void 
changeState(pid_t PID, int n){
}

void 
checkSession(pid_t PID, user_t * user){
	request_t request;
	int i;
	request.reqID = 3;
	request.par1 = 2;
	
	if(user->sessionID == -1)
		strcpy(request.message, "No tiene sesion asignada");
	else if (user->state = 2)
		strcpy(request.message, session[user->sessionID].name);
	sendRequest(request);	
}
