#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

class Listener;
class Socket;

class Listener{
	private:
		int socketDescriptor;
		struct sockaddr_in socketAddress;
	public:
		/**
		 * Constructor.
		 * Open a listener.
		 * 	int: the port number.
		 *	int: the queue lenght.
		 */
		Listener(int, int);
		/**
		 * Accept a socket in the connection request queue
		 * return
		 * 	Socket connection to this class
		 */
		Socket* acceptConnection();
		/** 
		 * Destructor
		 * Close this listener
		 */
		~Listener();
};

class Socket{
	private:
		int socketDescriptor;
	public:
		/** 
		 * for already created socket ie. accepted by a listener
		 */
		Socket(int);
		/**
		 * create a new socket connecting to a specific place.
		 * arguments
		 * string	IP adress
		 * int		port number
		 */
		Socket(std::string, int);
		/**
		 * read string from the socket
		 * return
		 * std::string a string read from stream
		 */
		std::string readLine();
		/**
		 * write string to socket
		 * arguments
		 * the content of the data
		 */
		void write(const char*, int);
		/**
		 *	Destructor
		 *  close this socket.
		 */
		~Socket();
};
