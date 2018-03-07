// EventLogger.cpp
/*
The Logger Class
Is a class to provide a logger that opens a socket connection and listens for messages to be logged into a log file.

Members:
LogFilename

Functions:
writeLogEvent(Event) -> writes a formatted LogEvent to the LogFile
openSocketForLogEventSubmission() -> opens a socket to receive messages that contain log event from other subsystems.
buildLogEventFromMessage(Message) -> formats a LogEvent from a Message

TYPE OF LOG MESSAGES
DEBUG	ID:0 
INFO	ID:1
WARNING	ID:2
ERROR	ID:3
FATAL	ID:4
*/



 
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

class EventLogger {
    public:
	EventLogger();
	static void* echoLogTest(void* param);
	void error(const char *msg);

    private:
	//int logEvent(LogEvent e);
	int openSocketForLogEventSubmission();
	int buildLogEventFromMessage(std::string message);
	std::string FileName;
};

//CONSTRUCTOR
EventLogger::EventLogger(){
	//Create the EventLog.txt file if it does not exist
	FileName = "EventLog.txt";

	if(openSocketForLogEventSubmission()){
		std::cout << "Socket Failure!!!" << std::endl;
	}
	// IF THIS FAILS IT NEEDS TO BE RECREATED...
	// IF IT KEEPS FAILING ABANDON LOGGING AND AVOID USING UP SYSTEM RESOURCES (MAYBE)

}
int main(){
	EventLogger MasterLog;
	std::cout << "something happened?" << std::endl;

}

int EventLogger::openSocketForLogEventSubmission(){
	int sockfd, newsockfd, portno = 9999;
     // create a TCP/IP socket
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     struct sockaddr_in serv_addr;
     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     /* setup the host_addr structure for use in bind call */
     // server byte order
     serv_addr.sin_family = AF_INET;

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;

     // port number to bind to
     serv_addr.sin_port = htons(portno);

     // This bind() call will bind  the socket to the current IP address on port
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
         error("ERROR on binding");
     }

     // This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a backlog queue
     // until accept() call accepts the connection.
     // Here, we set the maximum size for the backlog queue to 5.
     listen(sockfd,5);

     while((newsockfd = accept(sockfd, 0, 0))) {
         pthread_t t;
         pthread_create(&t, 0, echoLogTest, (void*)newsockfd);
     }
	return 0;
}


//int EventLogger::logEvent(LogEvent e){}


int EventLogger::buildLogEventFromMessage(std::string message){
	return 0;
}

void* EventLogger::echoLogTest(void* param){
    char buf[1024];
    int count;
    pthread_detach(pthread_self());
    intptr_t s = (intptr_t)param;
    while(( count = read(s, buf, 1023) > 0 )) {
	std::string message(buf), messageType, sys;
	messageType = message.substr(0,3);
	sys = message.substr(3,5);
	message = message.substr(8, message.length() - 8);
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << messageType<< " ["<< std::put_time(std::localtime(&now_c), "%F %T") << "] SYS:"<< sys <<" ==>" << message << "<=="<<std::endl;
	//std::cout << "Server received "<< buf<< std::endl;
        //printf("Server sending it back\n");
        //write(s, buf, strlen(buf));
	memset(&buf[0], 0, 1024);
    }

    close(s);
}

void EventLogger::error(const char *msg){
    perror(msg);
    exit(1);
}
