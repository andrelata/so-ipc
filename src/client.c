#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "client.h"


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
		if( connectionStatus == FALSE ){
			printf("Error en la conexion\n");
		}
	
	}while( connectionStatus == FALSE );
	
	printf("Conexion establecida\n");

}

int toSession(){

	printf("Que accion desea realizar?\n\t%d.Obtener informacion de las sesiones existentes\n\t%d.Unirse a una sesion\n\t%d.Crear una nueva sesion\n\t%d.Cambiar de usuario\n\t%d.Desconectarse\n\nPara elejir una accion ingrese el numero correspondiente:\n", GET_INFO, JOIN_SES, CREAT_SES, CH_USER, DISCONNECT);
	
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
		case GET_INFO:	
			string = getSessions();
			printf("%s", string);
			break;

		case JOIN_SES:	
			printf("Ingrese el ID de la sesion\n");
			int sessionID = 0;
			while(scanf("%d", &eleccion) < 1){
				getchar();
				printf("Ingrese un numero\n");
			}
			joinSession( sessionID );
			break;

		case CREAT_SES: 
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



