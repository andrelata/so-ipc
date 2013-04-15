#ifndef _DEFS_H_ 
#define _DEFS_H_ 

#define BUFSIZE 200

// enum??

#define ERROR		1
#define	OK		2
#define	CONNECT		3
#define	DISCONNECT	4
#define	GET_SESSIONS	5
#define	JOIN_SESSION	6
#define	EXIT_SESSION	7
#define CREATE_SESSION	8
#define	SEND_TEXT	9
#define	CHECK_PRICE	10
#define	CHANGE_STATE	11
#define	SEND_PRICE	12
#define	CHANGE_USER	13
#define CHECK_SESSION	14
#define ANS		15

#define NAME_LENGTH	20
#define	MESS_LENGTH	50
#define PRICE 0.01

//GONZA: la struct message me parece que esta al pedo
//con request alcanza porque almacena la misma info

//Andre: al final decidi dejar esta estructura porque no me parece muy prolijo que el request ande dando vueltas
// por las funciones de client.c solo para transportar un mensaje
typedef struct message {
	char nickname[NAME_LENGTH];
	char messageBody[MESS_LENGTH]; //(o algo asi, no quise repetir message)
	time_t time;
} message_t;

/*struct request{
	int reqID;
	void * struct;
}*/

typedef struct request{
	int reqID;
	pid_t PID;
	int par1;
	char name[NAME_LENGTH];
	char message[MESS_LENGTH];
	time_t time;
	float price;

} request_t;

#endif
