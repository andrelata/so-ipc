#ifndef _CLIENT_FIFO_H_ 
#define _CLIENT_FIFO_H_ 

/**
Parametros: ninguno
Obtiene los datos del usuario y llama a connect para conectarse con el servidor
*/
void makeConnection();

/**
Parametros: ninguno
Le presenta al usuario un menu con las opciones correspondientes a cuando el usuario no se encuentra
en una sesion, y llama a la funcion que corresponde segun la opcion elegida.
Retorna la opcion elegida por el usuario.
*/
int toSession();

/**
Parametros: ninguno
Funcion asignada a un thread que se crea cuando el usuario esta en una sesion. Esta funcion
llama a getRequest, la cual imprime en pantalla los mensajes de la sesion de chat y la tarifa
cuando el usaurio la solicita.
*/
void* printText(void* unused);

/**
Parametros: ninguno
Presenta al usuario un menu las opciones correspondientes al chat y llama a la funcion correspondiente.
Retorna
*/
void chat();

/**
Parametros: nickname
Envia un request al servidor para conectarse con el, abriendo los canales de comunicacion correspondientes.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int connectClient(char nick[]);

/**
Parametros: ninguno
Envia al servidor un request para desconectarse, cerrando los canales de comunicacion.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int disconnectClient();

/**
Parametros: ninguno
Obtiene las sesiones existentes en el servidor.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int getSessions();

/**
Parametros: ID de sesion
Incorpora al usuario a una sesion existente.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int joinSession(int sessionID);

/**
Parametros: nignuno
Desvincula al usuario de la sesion en la que este incorporado
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int exitSession();

/**
Parametros: nombre de la nueva sesion
Crea una nueva sesion.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int createSession(char name[]);

/**
Parametros: mensaje
Envia un mensaje de chat al servidor.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int sendText(char message[]);

/**
Parametros: ninguno
Envia un request al servidor para que envie un response con los datos de la tarifa.
No lee el response.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int checkPrice();

/**
Parametros: nuevo estado
Notifica al servidor de un cambio de estado del usuario.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int changeState(int state);

/**
Parametros: ninguno
Lee requests de tipo SEND_TEXT o SEND_PRICE e imprime en pantalla la informacion recibida.
Retorna OK si la operacion fue exitosa o ERROR si se produjo un error.
*/
int getRequest();

/**
Parametros: request
Envia un request al servidor. Depende del IPC que se use.
*/
void sendRequest(request_t req);

/**
Parametros: ninguno
Lee un request del servidor. Depende del IPC que se use.
Retorna el request en una estructura.
*/
request_t receiveRequest();

/**
Parametros: mensaje
Imprime en pantalla un mensaje de chat.
*/
void printMessage(message_t * message);

/**
Parametros: ninguno
Limpia el buffer del teclado;
*/
void clear();

#endif

