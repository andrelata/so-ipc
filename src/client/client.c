#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "../defs.h"
#include "client.h"


int main(){
	int option;

	do{
		makeConnection();
		do{
			option = toSession();
		}while(option == GET_SESSIONS);
		printf("option: %d\n", option);
		if(option==JOIN_SESSION || option == CREATE_SESSION){
			chat();
		}
	}while(option != CHANGE_USER);

	return 0;
	
}

void makeConnection(){
	int connectionStatus;
	do{
		char nick[20];
		printf("Elija su nickname para conectarse\n");
		while( scanf("%s", nick) < 1);
		connectionStatus = connect(nick);
		if( connectionStatus == ERROR ){
			printf("Error en la conexion\n");
		}
	
	}while( connectionStatus == ERROR );
	
	printf("Conexion establecida\n");

}

int toSession(){

	printf("Que accion desea realizar?\n\t%d.Obtener informacion de las sesiones existentes\n\t%d.Unirse a una sesion\n\t%d.Crear una nueva sesion\n\t%d.Cambiar de usuario\n\t%d.Desconectarse\n\nPara elejir una accion ingrese el numero correspondiente:\n", GET_SESSIONS, JOIN_SESSION, CREATE_SESSION, CHANGE_USER, DISCONNECT);
	
	int eleccion = 0;
	while ( scanf("%d", &eleccion) < 1){
		clear();
		printf("Ingrese un numero:\n");
	}
	char * string;
	switch(eleccion) {
		case GET_SESSIONS:	
			getSessions();
			break;

		case JOIN_SESSION:	
			printf("Ingrese el ID de la sesion\n");
			int sessionID = 0;
			while(scanf("%d", &eleccion) < 1){
				clear();
				printf("Ingrese un numero\n");
			}
			joinSession( sessionID );
			break;

		case CREATE_SESSION: 
			printf("Ingrese el nombre de la sesion\n");
			char sessionName[20];
			while(scanf("%s", sessionName) < 1){
				printf("Ingrese un nombre para la\n");
				clear();
			}
			createSession( sessionName );
			return CREATE_SESSION;
			break;

		case CHANGE_USER:

		case DISCONNECT:
			disconnect();
			break;

		default:
			printf("debe ingresar una de las opciones\n");
			eleccion = GET_SESSIONS;
			break;
	}
	return eleccion;
}

void* printText(void* unused){

	message_t * message;
	while( (message = readText()) != NULL){
		printMessage(message);
	}
	printf("Thread terminado\n");
	return NULL;
}


void chat(){
	pthread_t thread_id;
	/* Create a new thread. The new thread will run the printText
	function. */
	pthread_create (&thread_id, NULL, &printText, NULL);
	printf("Que accion desea realizar?\n\t%d.Ingresar texto\n\t%d.Cambiar de estado\n\t%d.Desconectarse\n\nPara elejir una accion ingrese el numero correspondiente:\n", SEND_TEXT, CHANGE_STATE, DISCONNECT);
	int eleccion = SEND_TEXT;
	int estado;
	while(eleccion!=DISCONNECT){
		while ( scanf("%d", &eleccion) < 1){
			clear();
			printf("Ingrese un numero:\n");
		}
		char string[50];
		switch(eleccion) {
			case SEND_TEXT:	
				printf("Ingrese texto\n");
				scanf("%s", string);
				sendText(string);
				break;
	
			case CHANGE_STATE:	
				printf("Ingrese el nuevo estado\n");
				int sessionID = 0;
				while(scanf("%d", &estado) < 1){
					clear();
					printf("Ingrese un numero\n");
				}
				changeState( estado );
				break;
	
			case DISCONNECT:
				disconnect();
				break;
	
			default:
				printf("debe ingresar una de las opciones\n");
				eleccion = GET_SESSIONS;
				break;
		}
	}
}

void printMessage(message_t * message){
	printf("%s dijo: %s\t\t a las %s\n", message->nickname, message->messageBody, ctime(&(message->time)));
}

int connect(char nick[]){
	request_t req;
	req.reqID = CONNECT;
	req.PID = getpid();
	strncpy(req.name, nick, NAME_LENGTH);
	if( openSChannel() == ERROR){
		return ERROR;
	}
	sendRequest(req);
	sleep(1);
	if( openCChannel() == ERROR){
		return ERROR;
	}
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}

	return OK;
}

int disconnect(){
	request_t req;
	req.reqID = DISCONNECT;
	req.PID = getpid();
	sendRequest(req);
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}

	if(closeChannel()==ERROR){
		printf("Error\n");
		printf("No se pudo cerrar el canal de comunicacion\n");
	}
	return OK;
}

int getSessions(){
	request_t req, reqRec;
	req.reqID = GET_SESSIONS;
	req.PID = getpid();
	sendRequest(req);
	reqRec = receiveRequest();
	if(reqRec.reqID==ERROR){
		printf("Error: %s\n", reqRec.message);
		return ERROR;
	}
	int i;
	if(reqRec.par1==0){
		printf("No hay ninguna sesion abierta\n");
	}else{
		printf("Las sesiones abiertas son:\n\nID\t\tNombre\n\n");
		for(i=0;i<reqRec.par1;i++){
			req = receiveRequest();
			printf("%d\t\t%s\n", req.par1, req.name);
		}
	}

	return OK;
	
	
}

int joinSession(int sessionID){
	request_t req;
	req.reqID = JOIN_SESSION;
	req.PID = getpid();
	req.par1 = sessionID;
	sendRequest(req);
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}
	return OK;
}

int exitSession(){
	request_t req;
	req.reqID = EXIT_SESSION;
	req.PID = getpid();
	sendRequest(req);
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}
	return OK;

}

int createSession(char name[]){
	request_t req;
	req.reqID = CREATE_SESSION;
	req.PID = getpid();
	strncpy(req.name, name, NAME_LENGTH);
	sendRequest(req);
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}
	return OK;

}

int sendText(char message[]){
	request_t req;
	req.reqID = SEND_TEXT;
	req.PID = getpid();
	strcpy(req.message, message);
	sendRequest(req);
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}
	return OK;
}

int checkPrice(){
	request_t req;
	req.reqID = CHECK_PRICE;
	req.PID = getpid();
	sendRequest(req);
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}
	return OK;
}

int changeState(int state){
	request_t req;
	req.reqID = CHANGE_STATE;
	req.PID = getpid();
	sendRequest(req);
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}
	return OK;
}

message_t * readText(){
	request_t req;
	req = receiveRequest();
	if(req.reqID == ERROR){
		return NULL;
	}else if(req.reqID != SEND_TEXT){
		printf("Error: %s\n", req.message);
	}
	message_t * message;
	strncpy(message->nickname,req.name, NAME_LENGTH);
	strncpy(message->messageBody, req.message, MESS_LENGTH);
	message->time = req.time;
	return message;
}

void
clear (void)
{    
	while ( getchar() != '\n' );
}



