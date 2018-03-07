// EventDispatcher.cpp

/*
Class to be incorporated by subsystems to send messages to SubsystemEventLogger
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> 

class EventDispatcher{
    public:
	EventDispatcher(std::string subsystemIdentifier);
	int sendDebugMessage(std::string m);
	int sendInfoMessage(std::string m);
	int sendWarningMessage(std::string m);
	int sendErrorMessage(std::string m);
	int sendFatalMessage(std::string m);
    private:		
	int sendMessage(std::string messageType, std::string message);
	std::string subsystem;
	int sockfd;
	int portno; 
};

int main(){
	EventDispatcher log("CAMR1");
	log.sendInfoMessage("Camera receiving data");
	std::cin.ignore();
	log.sendWarningMessage("Camera stopped receiving data");
	std::cin.ignore();
	log.sendErrorMessage("Camera feed terminated");
	std::cin.ignore();
	log.sendFatalMessage("Camera temperature below nominal operation levels. SHUTDOWN PROCEDURE INITIATED");
}

EventDispatcher::EventDispatcher(std::string subsystemIdentifier){
	subsystem = subsystemIdentifier;
	portno = 9999;
}

int EventDispatcher::sendDebugMessage(std::string message){
	return sendMessage("DBG", message);
}
int EventDispatcher::sendInfoMessage(std::string message){
	return sendMessage("INF", message);
}
int EventDispatcher::sendWarningMessage(std::string message){
	return sendMessage("WRN", message);
}
int EventDispatcher::sendErrorMessage(std::string message){
	return sendMessage("ERR", message);
}
int EventDispatcher::sendFatalMessage(std::string message){
	return sendMessage("FTL", message);
}
int EventDispatcher::sendMessage(std::string messageType, std::string message){
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return 0;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0){
        char buf[1024];
	std::string combinedMessage("");
	combinedMessage = messageType + subsystem + message;
        strncpy(buf, combinedMessage.c_str(), combinedMessage.length());
        write(sockfd, buf, strlen(buf));
        //int count = read(sockfd, buf, 1024);
        //printf("Got echo of %s from server\n", buf);
        shutdown(sockfd, SHUT_RDWR);
        memset(&buf[0], 0, 1024);
    }
    else
        return 0;	
}
