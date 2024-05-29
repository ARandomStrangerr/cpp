#include "HttpSession.h"
#include <vector>

// split function to support
std::vector<std::string> split (std::string blob, char preliminator){
	std::vector<std::string> tokens;
	int prev = 0;
	for (int i=0; i < blob.length(); i++){
		if (blob[i] == preliminator) {
			tokens.push_back(blob.substr(prev, i-prev+1));
			prev = i + 1;
		}
	}
	tokens.push_back(blob.substr(prev, blob.length() - prev));
	return tokens;
}

std::string trim(std::string blob){
	int start = 0, end = blob.length()-1;
	while ((blob[start] == ' ' || blob[start] == '\n') && start != end) start++;
	while ((blob[end] == ' ' || blob[end] == '\n') && end!= start) end--;
	return blob.substr(start, end-start);
}

HttpSession::HttpSession(int port, int backlog) : listener (port, backlog) {};

void HttpSession::start(){
	Socket socket = listener.accept();
	httpHandler(socket);
}

void HttpSession::httpHandler(Socket soc){
	// read the first line, which is route
	std::string firstLine = soc.readLine();
	std::vector<std::string> tokens = split(firstLine, '\n');
	JsonObject jsonObj;
	for (int i = 1; i < tokens.size(); i++) {
		std::vector<std::string> splitTokken = split(tokens[i], ':');
		jsonObj.add(trim(splitTokken[0]), trim(splitTokken[1]));
//		std::cout << splitTokken[0] << " || " << splitTokken[1] << std::endl;
	}
	std::cout << jsonObj << std::endl;
}

