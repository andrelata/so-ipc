#ifndef _CLIENT_FIFO_H_ 
#define _CLIENT_FIFO_H_ 

void makeConnection();

int toSession();

void* printText(void* unused);

void chat();

int connect(char nick[]);

int disconnect();

int getSessions();

int joinSession(int sessionID);

int exitSession();

int createSession(char name[]);

int sendText(char message[]);

int checkPrice();

int changeState(int state);

message_t * readText();

void sendRequest(request_t req);

request_t receiveRequest();

void printMessage(message_t * message);

void clear();

#endif

