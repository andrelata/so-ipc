#ifndef _SERVIDOR_H_ 
#define _SERVIDOR_H_ 

#include "../defs.h"

#define MAXSESSION 10

#define MAX_USERS 10

void connectClient(pid_t PID, char * name, user_t * user);

void disconnect(pid_t PID);

void checkSession(pid_t PID, user_t * user);

void getSession(pid_t PID);

void joinSession(pid_t PID, int n, user_t * user);

void exitSession(pid_t PID, user_t * user);

void createSession(pid_t PID, char * name);

void sentText(pid_t PID, char * message, user_t * user);

void checkPrice(pid_t PID, user_t * user);

void changeState(pid_t PID, int n, user_t * user);

void parserRequest(request_t request);

#endif
