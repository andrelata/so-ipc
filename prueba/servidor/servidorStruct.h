#ifndef _SERVIDORSTRUCT_H_ 
#define _SERVIDORSTRUCT_H_ 

typedef struct user {

	char nickname[20];
	int state; /*1 conectado, 2 en sesion*/
	int sessionID; // -1 si esta libre
	time_t time;
	pid_t userID;

} user_t;

typedef struct session {
	int id;
	char name[20];
	int users;
} session_t;


#endif