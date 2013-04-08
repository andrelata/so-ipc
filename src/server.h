#define	CONNECT		1
#define	DISCONNECT	2
#define	GET_SESSION	3
#define	JOIN_SESSION	4
#define	EXIT_SESSION	5
#define CREATE_SESSION	6
#define	SEND_TEXT	7
#define	CHECK_PRICE	8
#define	CHANGE_STATE	9
#define	SEND_PRICE	10

typedef struct user {
	char[20] nickname;
	int state;
	int sessionID; // #define
	time_t time;
	pid_t userID;
} user_t;

typedef struct session {
	int id;
	char [20] name;
	int users;
} session_t;

typedef struct message {
	char [20] nickname;
	char [50] messageBody; //(o algo asi, no quise repetir message)
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
	char[20] name;
	char[50] message;
	time_t time;
	float price;

} request_t;


