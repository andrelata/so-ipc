#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "server.h"


int main(){
	int option;

	do{

		makeConnection();
		do{
			option = toSession();
		}while(option != GET_INFO);
		if(option==JOIN_SES || option == CREAT_SES){
			option = chat();
		}
	}while(option == CH_USER);
	
}

void makeConnection(){
	bool connectionStatus;
	do{
		char nick[20];
		printf("Elija su nickname para conectarse\n");
		while( scanf("%s", nick) < 1);
		connectionStatus = connect(nick);
		if( connectionStatus == ERROR ){
			printf("Error en la conexion\n");
		}
	
	}while( connectionStatus == FALSE );
	
	printf("Conexion establecida\n");

}

int toSession(){

	printf("Que accion desea realizar?\n\t%d.Obtener informacion de las sesiones existentes\n\t%d.Unirse a una sesion\n\t%d.Crear una nueva sesion\n\t%d.Cambiar de usuario\n\t%d.Desconectarse\n\nPara elejir una accion ingrese el numero correspondiente:\n", GET_SESSIONS, JOIN_SESSION, CREATE_SESSION, CH_USER, DISCONNECT);
	
	int eleccion = 0;
	int cant = scanf("%d", &eleccion);
	/*while(cant!=0){
		getchar();
	}*/
	while( cant < 1 || eleccion > 4 || eleccion < 1){
		printf("Debe ingresar el numero que corresponde a una opcion\n");
		cant = scanf("%d", &eleccion);
		getchar();
	}
	char * string;
	switch(eleccion) {
		case GET_SESSIONS:	
			string = getSessions();
			printf("%s", string);
			break;

		case JOIN_SESSION:	
			printf("Ingrese el ID de la sesion\n");
			int sessionID = 0;
			while(scanf("%d", &eleccion) < 1){
				getchar();
				printf("Ingrese un numero\n");
			}
			joinSession( sessionID );
			break;

		case CREATE_SESSION: 
			printf("Ingrese el nombre de la sesion\n");
			char sessionName[20];
			while(scanf("%s", sessionName) < 1){
				printf("Ingrese un nombre para la\n");
				getchar();
			}
			createSession( sessionName );
			break;

		case CH_USER:

		case DISCONNECT:
			disconnect();
			break;

		default:
			printf("error!\n");
			break;
	}
	return eleccion;
}

void* printText(void* unused){

	char* message;
	message = readText();
	
}


void chat(){
	//pthread_t thread_id;
	/* Create a new thread. The new thread will run the printText
	function. */
	//pthread_create (&thread_id, NULL, &printText, NULL);
	


}

int connect(char nick[]){
	request_t req;
	req.reqID = CONNECT;
	req.PID = getPID();
	req.name = nick;
	send(req);
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
	req.PID = getPID();
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
	req.PID = getPID();
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
		printf("Las sesiones abiertas son:\n\nID\t\tNombre\n\n"):
		for(i=0;i<reqRec.par1;i++){
			req = recieveRequest();
			printf("%d\t\t%s\n", req.par1, req.name);
		}
	}

	return OK;
	
	
}

int joinSession(int sessionID){
	request_t req;
	req.reqID = JOIN_SESSION;
	req.PID = getPID();
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
	req.PID = getPID();
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
	req.PID = getPID();
	req.name = name;
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
	req.PID = getPID();
	req.message = message;
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
	req.PID = getPID();
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
	req.PID = getPID();
	sendRequest(req);
	req = receiveRequest();
	if(req.reqID==ERROR){
		printf("Error: %s\n", req.message);
		return ERROR;
	}
	return OK;
}



