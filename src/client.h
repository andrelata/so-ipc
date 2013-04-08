#define	FALSE	0
#define TRUE	1
#define bool	int

//options
#define	GET_INFO	1
#define	JOIN_SES	2
#define	CREAT_SES	3
#define	CH_USER		4
#define	DISCONNECT	5

bool connect(char* nickname);

void disconnect();

char * getSessions();

void joinSession(int sessionID);

void createSession(char* name);

char * readText();
