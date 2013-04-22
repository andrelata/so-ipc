#ifndef _SERVIDORSTRUCT_H_ 
#define _SERVIDORSTRUCT_H_ 

#include "../defs.h"

typedef struct user {
	pid_t userID;		
	char nickname[NAME_LENGTH];
	int state;
	int sessionID; 
	time_t time;
} user_t;

typedef struct session {
	int id;		
	char name[NAME_LENGTH];
	int users;
} session_t;


#endif
