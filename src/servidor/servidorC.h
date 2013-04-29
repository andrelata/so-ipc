#ifndef _SERVIDORC_H_ 
#define _SERVIDORC_H_ 

#include "../defs.h"
#include "./servidorStruct.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define MAXSESSION 10

#define MAX_USERS 20

#define SERVER "/message0"
#define SEMAPH "/mutex0"

#define SIZE 1000

/**
Parametros: PID del usuario, nickname
crea el fifo del cliente con el PID del usuario,
e inicializa un usuario con el nickname y el PID.
le manda un request al usuario notificando resultados la operacion
*/
void connectClient(pid_t PID, char * name);

/**
Parametros: PID del usuario
cierra el fifo dle cliente .
le manda un request al usuario notificando resultados la operacion
y la tarifa que debe abonar.
*/
void disconnect(pid_t PID);

/**
Parametros: PID del usuario
le manda un request al usuario notificando en que sesion se encuentra
*/
void checkSession(pid_t PID);

/**
Parametros: PID del usuario
le manda un request al usuario con todas las sesiones creadas. 
*/
void getSession(pid_t PID);

/**
Parametros: PID del usuario, id de la sesion
une el usuario a la sesion elegida si el id existe.
le manda un request al usuario notificando resultados la operacion
*/
void joinSession(pid_t PID, int n);

/**
Parametros: PID del usuario.
desloguea al usuario de la sesion en la que se encuentra,
si es el ultimo elimina la sesion.
le manda un request al usuario notificando resultados la operacion
*/
void exitSession(pid_t PID);

/**
Parametros: PID del usuario, nombre de la sesion
crea una nueva sesion.
le manda un request al usuario notificando resultados la operacion
*/
void createSession(pid_t PID, char * name);

/**
Parametros: PID del usuario, mensaje
le manda un request a los usuarios que se encuentra en la sesion con
el mensaje enviado.
*/
void sentText(pid_t PID, char * message);

/**
Parametros: PID del usuario
le manda un request al usuario con el monto de la tarifa hasta el
momento.
*/
void checkPrice(pid_t PID);

/**
Parametros: PID del usuario, nuevo estado
cambia el estado del usuario por el nuevo.
le manda un request al usuario notificando resultados la operacion
*/
void changeState(pid_t PID, int n);

/**
Parametros: request
Parsea el request que le manda el usuario 
y le devuelve un request segun la accion realiza
con la operacion que corresponda
*/
void parserRequest(request_t request);

request_t receiveRequest();

int createServerSH();

void initmutex(void);

void fatal(char *s);

void enter(void);

void leave(void);

#endif
