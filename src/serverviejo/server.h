// enum??

#define ERROR		0
#define	OK		1
#define	CONNECT		2
#define	DISCONNECT	3
#define	GET_SESSIONS	4
#define	JOIN_SESSION	5
#define	EXIT_SESSION	6
#define CREATE_SESSION	7
#define	SEND_TEXT	8
#define	CHECK_PRICE	9
#define	CHANGE_STATE	10
#define	SEND_PRICE	11
#define	CHANGE_USER	12

#define NAME_LENGTH	20
#define	MESS_LENGTH	50


typedef struct user {
	char nickname[NAME_LENGTH];
	int state;
	int sessionID; // #define
	time_t time;
	pid_t userID;
} user_t;

typedef struct session {
	int id;
	char name[NAME_LENGTH];
	int users;
} session_t;

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


