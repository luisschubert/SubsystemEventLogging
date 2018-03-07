# SubsystemEventLogging
>programs for communicating system-events using sockets

*All programs that monitor subsystems instantiate the EventDispatcher to report information to the system log.*

> one-to-many relationship exists between EventLogger and EventDispatcher.

## EventDispatcher.cpp
contains EventDispatcher class to be used by subsystems to send messages to the log.
```
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
```

## EventLogger.cpp
contains EventLogger class to be used as the receiver of log messages to compile them into respective files.
```
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
```

#### Event Types
- DEBUG
- INFO
- WARNING
- ERROR
- FATAL

### TODO:
- [ ] implement filewriter class and add to EventLogger 
- [ ] provide header files to include into subsystem monitor programs 

#### Additional Thoughts:
- Using UDP instead of TCP???
