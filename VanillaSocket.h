#include "iostream"
#include "sys/socket.h"
#include "netinet/in.h"
#include "cstring"
#include "unistd.h"

class Listener;
class Socket;

class Listener{
	private:
		int socketDescriptor;
		struct sockaddr_in socketAddress;
	public:
		// Open a listener</br>
		// 	int: the port number
		//	int: the queue lenght
		Listener(int, int);
		// accept a socket in the connection request queue
		Socket accept();
		// close the listener
		void disconnect();
};

class Socket{
	private:
		int socket;
	public:
		// for already created socket ie. accepted by a socket
		Socket(int);
		// create a new socket connecting to a specific place
		Socket(std::string, int);
		// read string from the socket
		std::string read();
		// write a string to the socket
		void write(std::string*);
};
