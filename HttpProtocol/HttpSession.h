#include "Socket.h"
#include <map>
#include <thread>

class HttpSession {
	private:
		std::map<std::string, std::string> pathMap;
		Listener listener;
	public:
		HttpSession();
		void addGetRoute();
		void start();
		void httpHandler(Socket);
};
