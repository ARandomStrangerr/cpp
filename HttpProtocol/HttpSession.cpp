#include "HttpSession.h"
#include <vector>

// split function to support
std::vector<std::string> split (std::string blob, char preliminator){
	std::vector<std::string> tokens;
	int prev = 0;
	for (int i=0; i < blob.length(); i++){
		if (blob[i] == preliminator) {
			tokens.push_back(blob.substr(prev, i));
			prev = i;
		}
	}
	return tokens;
}

HttpSession::HttpSession() : listener(80, 10) {};

void HttpSession::start(){
	Socket socket = listener.accept();
}
void HttpSession::httpHandler(Socket soc){
	// read the first line, which is route
	std::string firstLine = soc.readLine();
	std::vector<std::string> tokens = split(firstLine, ' ');	
}

