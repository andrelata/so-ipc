/**/

#define BUFSIZE 200

typedef struct request{

	char reqID;
	pid_t PID;
	int par1;
	//char[20] name = "";
	//char[50] message = "";
	time_t time;
	float price;

} request_t;
