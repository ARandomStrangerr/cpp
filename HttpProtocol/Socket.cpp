#include "Socket.h"
#include <sys/socket.h>

Socket::Socket(int sd){
	this->sd = sd; 
}

Socket::Socket(std::string addr, int port){
	// same porcedure as creating a listener
	// create a socker descriptor
	this->sd = socket(AF_INET, SOCK_STREAM, 0);
	// create an obj to describe the socket
	struct sockaddr_in sockAddrDesc;
	sockAddrDesc.sin_family = AF_INET;
	sockAddrDesc.sin_port = htons(port);
	// convert the given ip addr from string to binary
	inet_pton(AF_INET, addr.c_str(), &sockAddrDesc.sin_addr.s_addr);
	// connect to the desinated address
	connect(sd, (struct sockaddr*) &sockAddrDesc, sizeof(sockAddrDesc));
}

void Socket::writeLine(std::string str){
	const char* cStr = str.c_str();
	send(sd, cStr, sizeof(cStr), 0);
}

std::string& Socket::readLine(){
	char buffer[1024] = {0};
	std::string* returnStr = new std::string();
	while (returnStr->length()==0 || (*returnStr)[returnStr->length()-1] != '\n'){
		read(this->sd, buffer, 1023);
		*returnStr += buffer;
	}
	return *returnStr;
}

Listener::Listener(int port, int backlog){
	/* int - socket(int, int, int) - create a socket descriptor
	 * int - domain: communication domain, require <netinet/in.h>
	 * 	can be value: AF_LOCAL, AF_INET, AF_INET6
	 * int - type: communication type, require <netinet/in.h>
	 * 	can be value: SOCK_STREAM, SOCK_DGRAM
	 * int - protocol: protocol value, for internet = 0
	 */	
	sd = socket(AF_INET, SOCK_STREAM,0);
	
	/*
	 * sockaddr_in, a structure describe the socket address
	 * we have to setup:
	 * sin_family: the domain of the socket, same as domain of socket descriptor
	 * sin_port: the port which the socket operate on
	 * sin_addr: ip address which this socket will accept
	 */
	sockAddrDesc.sin_family = AF_INET; // listener accept ipv4
	sockAddrDesc.sin_port = htons(port); // declare the port for the listener
	sockAddrDesc.sin_addr.s_addr = INADDR_ANY; // listener accpet any ip address
	
	/*
	 * int - bind(int, struct sockaddr*, int) - attempt to bind the socket descriptor created above to a port with specified information in the struct sockaddr
	 * int - sd: the socket descriptor created above
	 * struct sockaddr* - socketAddress: information to configurate the port
	 * int - size: size of the socketAddress above
	 */
	int bd = bind(sd, (struct sockaddr*) &sockAddrDesc, sizeof(sockAddrDesc));

	/*
	 * int - listen(int, int) - attempt to listener by using the port binded with the socket descriptor.
	 * int - sd: the socket descriptor which is decalred above
	 * int - backlog: the number of connection can be queue in line.
	 */
	int listener = listen(sd, backlog);
}

Socket& Listener::accept(){
	socklen_t addrLen = sizeof(sockAddrDesc);
	Socket* sock = new Socket(::accept(sd, (struct sockaddr*) &sockAddrDesc, &addrLen));
	return *sock;
}
