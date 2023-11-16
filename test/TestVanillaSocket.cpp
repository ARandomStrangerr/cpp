#include "../VanillaSocket.h"
#include <thread>
#include <unistd.h>

void listener(){
	Listener listener (8888, 5); // open a listener at port 8888 with backlog 5
	Socket socket = listener.acceptConnection(); // accept incoming socket
	std::string str = "hello\n";
	socket.write(str.c_str(), str.length());
}

void socketTester(){
	Socket socket("127.0.0.1", 8888);// open a socket at port 8888 with backlog 5
	std::cout<<socket.readLine()<<std::endl;
}

int main(){
	std::thread t1(listener);
	usleep(10000);
	std::thread t2(socketTester);
	t1.join();
	t2.join();
}
