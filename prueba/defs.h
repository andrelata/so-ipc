#ifndef _DEFS_H_ 
#define _DEFS_H_ 

#define BUFSIZE 200

typedef struct request{

	char reqID;
	pid_t PID;
	int par1;
	char name[20];
	char message[50];
	time_t time;
	float price;

} request_t;

#endif
