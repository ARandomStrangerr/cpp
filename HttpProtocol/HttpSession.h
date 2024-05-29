#include "Socket.h"
#include "Json.h"
#include <map>
#include <thread>

class HttpSession {
	private:
		std::map<std::string, std::string> pathMap;
		Listener listener;
	public:
		HttpSession(int, int);
		void addGetRoute();
		void start();
		void httpHandler(Socket);
};
