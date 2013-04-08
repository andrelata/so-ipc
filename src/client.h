#define	FALSE	0
#define TRUE	1
#define bool	int

bool connect(char* nickname);

void disconnect();

char * getSessions();

void joinSession(int sessionID);

void createSession(char* name);

char * readText();
