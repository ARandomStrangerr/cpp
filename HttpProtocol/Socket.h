#include <string>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Listener;
class Socket;

class Listener {
	private:
		int sd;
		struct sockaddr_in sockAddrDesc;
	public:
		// constructors
		Listener(int, int); // open a listener at the given port with specified queue length
		// destructor
		~Listener();
		// functions
		Socket& accept();
};

class Socket {
	private:
		int sd;
	public:
		/**
		 * constructor to wrap a file descriptor
		 * @params:
		 * int - a file descriptor
		 */
		Socket(int);

		/**
		 * constructor create a new socket descriptor
		 * @params:
		 * std::string - ipv4 name
		 * int - port
		 */
		Socket(std::string, int); // create a new socket from ipv4 and port

		// destructor
		~Socket();

		/**
		 * write a line into output buffer then flush it. new-line character will be appened at the end
		 * @params:
		 * std::string - the string to flush into the buffer
		 */
		void writeLine(std::string); // write a line of string to the buffer then flush it

		/**
		 * read a line from input buffer. this can be terminated when a new-line character is meet.
		 * @return:
		 * std::string& - a string represent input data
		 */
		std::string& readLine(); // read a line of string
};

