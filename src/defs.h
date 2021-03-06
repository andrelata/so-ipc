#ifndef _DEFS_H_ 
#define _DEFS_H_ 

#define BUFSIZE 200

enum requestTypes { ERROR = 1, OK, CONNECT, DISCONNECT, GET_SESSIONS, JOIN_SESSION, EXIT_SESSION, CREATE_SESSION, SEND_TEXT, CHECK_PRICE, CHANGE_STATE, SEND_PRICE, CHANGE_USER, CHECK_SESSION, ANS};

#define EMPTY		-1

/*#define ERROR		1
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
#define ANS		15*/

#define NAME_LENGTH	20
#define	MESS_LENGTH	50
#define PRICE 0.01

typedef struct message {
	char nickname[NAME_LENGTH];
	char messageBody[MESS_LENGTH]; //(o algo asi, no quise repetir message)
	time_t time;
} message_t;

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
