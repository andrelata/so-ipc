#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <time.h>

#include "../defs.h"
#include "./servidor_IPC.h"
#include "./servidor.h"
#include "./servidorStruct.h"

session_t session[MAXSESSION];
user_t user[MAX_USERS];

int
main(int argc, char **argv)
{
	char buf[BUFSIZE];
	char * client_name = "/tmp/fifocli";
	request_t request;

	if(createServerChannel() == -1)
		return 0;


	//inicializacion de sesiones
	int i;
	for(i = 0; i < MAXSESSION; i++)
	{
		session[i].id = EMPTY; /*indica que la session esta vacia*/
	}

	//inicializacion de usuarios

	for(i = 0; i < MAX_USERS; i++)
	{
		user[i].userID = EMPTY;
	}

	printf("Iniciando el servidor. \n");

	while(1){
		request = receiveRequest();
		parserRequest(request);
	}
}

void
parserRequest(request_t request){
	
	switch(request.reqID){
		case CONNECT: 
			connectClient(request.PID, request.name);
			break;
		case DISCONNECT:
			disconnectClient(request.PID);
			break;
		case GET_SESSIONS:
			getSession(request.PID);
			break;
		case JOIN_SESSION:
			joinSession(request.PID, request.par1);
			break;

		case EXIT_SESSION:
			exitSession(request.PID);
			break;
		case CREATE_SESSION:
			createSession(request.PID, request.name);
			break;
		case SEND_TEXT: 
			sentText(request.PID, request.message);
			break;
		case CHECK_PRICE:
			checkPrice(request.PID);
			break;
		case CHANGE_STATE:
			changeState(request.PID, request.par1);
			break;
		case CHECK_SESSION:
			checkSession(request.PID);
			break;
		default:
			break;
	}
}

void 
connectClient(pid_t PID, char * name){

	request_t request;

	request.PID = PID; 

	int i;

	if( createChannel(PID) == -1 ){
		request.reqID = ERROR;
	}else{
		request.reqID = OK;
	}

	for(i = 0; i < MAX_USERS; i++)
	{
		if(user[i].userID != EMPTY && strcmp(user[i].nickname, name) == 0)
		{
			request.reqID = ERROR;
			//sprintf(string,"El nickname %s ya esta siendo utilizado, elija otro\n", name);
			//strcpy(request.message,string);
			printf("Error message: %s req.PID: %d\n", request.message, request.PID);
			sendRequest(request);
			closeCChannel(PID);
			return;
		}
	}

	int j=-1;

	for(i = 0; i < MAX_USERS && j==-1; i++)
	{
		if(user[i].userID == EMPTY)
		{
			j=i;
		}
	}

	if(j==-1){
		request.reqID = ERROR;
		strcpy(request.message,"Se ha alcanzado el límite de usuarios, vuelva mas tarde\n");
		sendRequest(request);
		closeCChannel(PID);
		return;
	}

	strcpy(user[j].nickname, name);
	user[j].userID = PID;
	user[j].state = 1; /*conectado = 1*/
	user[j].sessionID = EMPTY; /*no tiene session asignada = -1*/
	user[j].time = time(NULL);
	
	sendRequest(request);	
}

void 
disconnectClient(pid_t PID){

	request_t request;
	int i;

	request.PID = PID;

	int k = getUserIndex(PID);

	user[k].userID = EMPTY;

	closeCChannel(PID);
}

void getSession(pid_t PID)
{
	request_t request;
	int i;
	int count = 0;
	char * sessions;

	request.PID = PID;
	
	for(i = 0; i < MAXSESSION; i++)
	{
		if(session[i].id != EMPTY)
		{
			count++;
		}
		request.reqID = OK;
	}
	if(count == 0)
	{
		request.par1 = ERROR;
		strcpy(request.message,"No hay sesiones creadas");
		
	}
	request.par1 = count;
	sendRequest(request);

	for(i = 0; i < MAXSESSION; i++)
	{
		if(session[i].id != EMPTY)
		{
			request.par1 = session[i].id;
			strcpy(request.name,session[i].name);
			sendRequest(request);
		}
	}
		
}

void 
joinSession(pid_t PID, int n){
	
	request_t request;
	int i;

	request.PID = PID;

	int k = getUserIndex(PID);


	for(i = 0; i < MAXSESSION; i++)
	{
		if(session[i].id == n)
		{
			user[k].sessionID = n;
			user[k].state = 2; /*en sesion*/
			session[i].users += 1; 
			request.reqID = OK;
			break;
		}
	}
	if(i == MAXSESSION)
	{
		request.reqID = ERROR;
		strcpy(request.message, "No existe esa sesion");
	}
	sendRequest(request);
	
}

void 
exitSession(pid_t PID){
	request_t request;
	int i, j;

	request.PID = PID;

	int k = getUserIndex(PID);
	
	if(user[k].sessionID == EMPTY){
		request.reqID = ERROR;
		strcpy(request.message,"El usuario no se encuentra en ninguna sesion\n");
	}else 
	{
		if(session[user[k].sessionID].users == 1)
		{
			session[user[k].sessionID].id = EMPTY;
			session[user[k].sessionID].users = 0;
		}else {
			session[user[k].sessionID].users -= 1;
		}
		request.reqID = OK;
	}
	user[k].sessionID = EMPTY;
	request.reqID = ERROR;
	sendRequest(request);
	
}

void 
createSession(pid_t PID, char * name){
	request_t request;
	int i;
	char * s;

	int k = getUserIndex(PID);

	request.PID = PID;

	request.reqID = OK;

	for(i = 0; i < MAXSESSION; i++)
	{
		if(strcmp(session[i].name, name) == 0){
			request.reqID = ERROR;
			strcpy(request.message,"Ya existe una sesion con ese nombre");
			break;
		}
		if(session[i].id == EMPTY)
			break;	
	}
	if(request.reqID != ERROR && i < MAXSESSION)
	{
		session[i].id = i;
		strcpy(session[i].name,name);
		session[i].users = 1;
		user[k].sessionID = i;
	}
	
	sendRequest(request);
	
}

void 
sentText(pid_t PID, char * message){

	int i;
	
	request_t request;

	request.PID = PID;

	int k = getUserIndex(PID);
	
	if(user[k].sessionID != EMPTY)
	{
		request.reqID = SEND_TEXT;
		strcpy(request.name, user[k].nickname);
		strcpy(request.message, message);
		request.time = time(NULL);
		
	}else
	{
		request.reqID = ERROR;
		strcpy(request.message, "No puede mandar un mensaje si no esta en sesion");
	}
	
	for(i = 0; i < MAX_USERS; i++)
	{
		
		if(user[i].userID != EMPTY && user[i].sessionID == user[k].sessionID)
		{
			request.PID = user[i].userID;
			sendRequest(request);
		}
	}
	
}

void 
checkPrice(pid_t PID){
	request_t request;

	request.PID = PID;

	int k = getUserIndex(PID);
	
	request.reqID = SEND_PRICE;
	time_t actualTime = time(NULL);
	double time = difftime(actualTime, user[k].time);
	request.price = time * PRICE;
	
	sendRequest(request);
}

void 
changeState(pid_t PID, int n){
	request_t request;

	request.PID = PID;

	int k = getUserIndex(PID);
	
	user[k].state = n;
	request.reqID = OK;
	
	sendRequest(request);
}

void 
checkSession(pid_t PID){
	request_t request;
	int i;

	request.PID = PID;
	request.reqID = ANS;

	int k = getUserIndex(PID);
	
	if(user[k].sessionID == EMPTY)
		strcpy(request.message, "No tiene sesion asignada");
	else if (user[k].state = 2)
		strcpy(request.message, session[user->sessionID].name);
	sendRequest(request);	
}

int
getUserIndex(int pid){
	int i;
	int k=-1;

	for(i = 0; i < MAX_USERS && k==-1; i++)
	{
		if(user[i].userID == pid)
		{
			k=i;
		}
	}
	return k;
}

