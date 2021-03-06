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
	int option = CHANGE_USER;

	do{
		if(option == CHANGE_USER){
			makeConnection();
		}
		do{
			option = toSession();
		}while(option == GET_SESSIONS);
		if(option==JOIN_SESSION || option == CREATE_SESSION){
			chat();
		}
	}while(option != DISCONNECT);

	return 0;
	
}

void makeConnection(){
	int connectionStatus;
	do{
		char nick[20];
		printf("Elija su nickname para conectarse\n");
		while( scanf("%s", nick) < 1);
		connectionStatus = connectClient(nick);
		if( connectionStatus == ERROR ){
			printf("Error en la conexion\n");
		}
	
	}while( connectionStatus == ERROR );
	
	printf("Conexion establecida\n");

}

int toSession(){

	printf("Que accion desea realizar?\n\t%d.Obtener informacion de las sesiones existentes\n\t%d.Unirse a una sesion\n\t%d.Crear una nueva sesion\n\t%d.Consultar Tarifa\n\t%d.Cambiar de usuario\n\t%d.Desconectarse\n\nPara elegir una accion ingrese el numero correspondiente:\n", GET_SESSIONS, JOIN_SESSION, CREATE_SESSION, CHECK_PRICE, CHANGE_USER, DISCONNECT);
	
	int eleccion = 0;
	while ( scanf("%d", &eleccion) < 1){
		clear();
		printf("Ingrese el numero de opcion:\n");
	}
	char * string;
	switch(eleccion) {
		case GET_SESSIONS:	
			getSessions();
			break;

		case JOIN_SESSION:	
			printf("Ingrese el ID de la sesion\n");
			int sessionID = 0;
			while(scanf("%d", &sessionID) < 1){
				clear();
				printf("Ingrese un numero\n");
			}
			if(joinSession( sessionID )==ERROR){
				eleccion=GET_SESSIONS;
			}
			break;

		case CREATE_SESSION: 
			printf("Ingrese el nombre de la sesion\n");
			char sessionName[20];
			clear();
			fgets(sessionName, sizeof sessionName, stdin);
			if(createSession(sessionName) == ERROR){
				eleccion=GET_SESSIONS;
			}
			break;

		case CHECK_PRICE:
			checkPrice();
			getRequest();
			break;

		case CHANGE_USER:

		case DISCONNECT:
			checkPrice();
			getRequest();
			disconnectClient();
			break;

		default:
			printf("debe ingresar una de las opciones\n");
			eleccion = GET_SESSIONS;
			break;
	}
	return eleccion;
}

void* printText(void* unused){

	while( (getRequest()) != ERROR){
	}
	return NULL;
}


void chat(){
	pthread_t thread_id;
	pthread_create (&thread_id, NULL, &printText, NULL);
	int eleccion = SEND_TEXT;
	int estado;
	while(eleccion!=EXIT_SESSION){
		printf("Que accion desea realizar?\n\t%d.Ingresar texto\n\t%d.Consultar tarifa\n\t%d.Salir de sesion\n\nPara elegir una accion ingrese el numero correspondiente:\n", SEND_TEXT, CHECK_PRICE, EXIT_SESSION);
		while ( scanf("%d", &eleccion) < 1){
			clear();
			printf("Ingrese el numero de opcion:\n");
		}
		char string[50];
		switch(eleccion) {
			case SEND_TEXT:	
				printf("Ingrese texto\n");
				clear();
				fgets(string, sizeof string, stdin);
				//scanf("%s", string);
				sendText(string);
				break;
	
			/*case CHANGE_STATE:	
				printf("Ingrese el nuevo estado\n");
				int sessionID = 0;
				while(scanf("%d", &estado) < 1){
					clear();
					printf("Ingrese un numero\n");
				}
				changeState( estado );
				break;*/

			case CHECK_PRICE:
				checkPrice();
				break;
	
			case EXIT_SESSION:
				exitSession();
				break;
	
			default:
				printf("debe ingresar una de las opciones\n");
				eleccion = GET_SESSIONS;
				break;
		}
	}
}

void printMessage(message_t * message){
	printf("%s dijo: %s\t\t\t %s", message->nickname, message->messageBody, ctime(&(message->time)));
}

int connectClient(char nick[]){
	request_t req, req2;
	req.reqID = CONNECT;
	req.PID = getpid();
	strncpy(req.name, nick, NAME_LENGTH);
	if( openCChannel() == ERROR){
		return ERROR;
	}
	sleep(1);
	if( openSChannel() == ERROR){
		return ERROR;
	}
	sendRequest(req);
	sleep(1);
	req2 = receiveRequest();
	if(req2.reqID==ERROR){
		printf("Error: %s\n", req2.message);
		return ERROR;
	}

	return OK;
}

int disconnectClient(){
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
			if(req.reqID == OK ){
				printf("%d\t\t%s\n", req.par1, req.name);
			}
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
	/*req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}*/
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
	/*req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}*/
	return OK;
}

int checkPrice(){
	request_t req;
	req.reqID = CHECK_PRICE;
	req.PID = getpid();
	sendRequest(req);
	/*req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}*/
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

int getRequest(){
	request_t req;
	req = receiveRequest();
	if(req.reqID == ERROR){
		return ERROR;
	}else if(req.reqID == SEND_PRICE){
		printf("La tarifa actual es de %f pesos\n", req.price);
		return OK;
	}else if(req.reqID != SEND_TEXT){
		printf("Error: %s\n", req.message);
		return ERROR;
	}
	message_t message;
	strncpy(message.nickname,req.name, NAME_LENGTH);
	strncpy(message.messageBody, req.message, MESS_LENGTH);
	message.time = req.time;
	printMessage(&message);
	return OK;
}

void
clear (void)
{    
	while ( getchar() != '\n' );
}



