#include "VanillaSocket.h"

Listener::Listener(int port, int queueLenght){
	// socket(int domain, int type, int protocol) description of the socket
	// domain:		communication domain, require <netinet/in.h>
	//				AF_LOCAL for local host
	//				AF_INET for ipv4
	//				AF_INET6 for ipv6
	// type:		communication type require <netinet/in.h>
	// 				SOCK_STREAM for TCP (reliable) connection
	// 				SOCK_DGRAM for UDP (unreliable) connection
	// protocol:	protocol value for Internet Protocol which is 0
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	// int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len)
	// optional step to reuse of addres and port
	// set the option specified by 'option_name' argument
	// refer to the link at 'option_name' for details
	// socket: 			socket descriptor created above
	// level: 			protocol level
	// 					SOL_SOCKET to set option at socket level
	// option_name:		https://pubs.opengroup.org/onlinepubs/000095399/functions/setsockopt.html refer to this for available options
	// option_value:	
	// option_len: 		the length of the 'option_value'
	int otp = 1;
	int socketOption = setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &otp, sizeof(otp));

	// int bind(int socketDescriptor, struct sockaddr*, int sockaddrSize) bind the above socket to a port
	// socketDescriptor:		the socket descriptor which is created above
	// sockaddr:				a structure which contains variables about
	// 		sin_family:			the type of socker descriptor
	// 		sin_port:			the port which socket descriptor operates on
	// 		sin_addr.s_addr:	type of ip address which this socket accept
	// socketaddrSize:			the size of the above argument
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = port;
	socketAddress.sin_addr.s_addr = INADDR_ANY;
	int bindDescriptor = bind(socketDescriptor, (struct sockaddr*) &socketAddress, sizeof(socketAddress));

	// int listen(int socketDescriptor, int backlog) start listen to incoming connection on the bind port
	// socketDescriptor:	the socket descriptor created above
	// backlog:				the maximum size of the queue for pending connection.
	// 						if a connection arrive when the queue is full, the connection will be refuse; which return error ECONNREFUSED
	int listener = listen(socketDescriptor, 3);
}

Socket Listener::accept(){
	socklen_t addrSize = sizeof(socketAddress);
	int socket = ::accept(socketDescriptor, (struct sockaddr*) &socketAddress, &addrSize);
	return Socket(socket);
}

void Listener::disconnect(){
	close(socketDescriptor);
}

Socket::Socket(int socket){
	this->socket = socket;
}

void Socket::write(std::string* sendString){
	send(socket, sendString, sendString -> size(), 0);
}

std::string Socket::read(){
	char* buffer[1024] = {NULL}; // decalre the bufer to hold the incomming data
	std::string bufferString = ""; // the read buffer will be concaternated here and return
	while (bufferString.length() == 0 || bufferString[bufferString.length() - 1] != '\n'){
		::read(socket, buffer, sizeof(buffer)); // read from buffer
		bufferString += *buffer; // concaternate into the string
	}
	return bufferString;
}
