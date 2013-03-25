#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "client.h"
#include "fifo.h"


int main(){

	makeConnection();
	toSession();
	chat();
	
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

	printf("Que accion desea realizar?\n\t1.Obtener informacion de las sesiones existentes\n\t2.Unirse a una sesion\n\t3.Crear una nueva sesion\n\t4.Desconectarse\n\nPara elejir una accion ingrese el numero correspondiente:\n");
	
	int eleccion = 0;
	int cant = scanf("%d", &eleccion);
	while(cant--!=0)
		getchar();
	while( cant < 1 || eleccion > 3 || eleccion < 1){
		printf("Debe ingresar un numero del 1 al 3\n");
		cant = scanf("%d", &eleccion);
		getchar();
	}
	char * string;
	switch(eleccion) {
		case 1:	string = getSessions();
			printf("%s", string);
			break;

		case 2:	printf("Ingrese el ID de la sesion\n");
			int sessionID = 0;
			while(scanf("%d", &eleccion) < 1){
				getchar();
				printf("Ingrese un numero\n");
			}
			joinSession( sessionID );
			break;

		case 3: printf("Ingrese el nombre de la sesion\n");
			char sessionName[20];
			while(scanf("%s", sessionName) < 1){
				printf("Ingrese un nombre para la\n");
				getchar();
			}
			createSession( sessionName );
			break;

		case 4:	disconnect();
			break;

		default:
			printf("error!\n");
			break;
	}
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



