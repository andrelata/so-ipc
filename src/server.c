



int main(){
	
	openChannel();
	while(1){
		request_t req = readRequest();
		switch(request_t.reqID){
			case CONNECT:
				connect(req.PID, req.name);
				break;

			case DISCONNECT:
				disconnect(req.ID);
				break;
			case GET_SESSIONS:
				
			case JOIN_SESSION:
			case EXIT_SESSION:
			case CREATE_SESSION:
			case SEND_TEXT:
			case CHECK_PRICE:
			case CHANGE_STATE:
			case SEND_PRICE:
			default:
		}
	}
}


int sendPrice(float price){

}

int sendText(char nick[], char messBody[], time_t time){

}
