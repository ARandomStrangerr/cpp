#include <iostream>
#include <thread>
#include <unistd.h>
#include "VanillaSocket.h"

using namespace std;

void listenerThread(){
	Listener* listener = new Listener(9999, 2);
	cout<<"LISTENER: open listener at port 9999"<<endl;
	Socket* socket1 = listener->acceptConnection();
	cout<<"LISTENER: accepted socket1"<<endl;
	Socket* socket2 = listener->acceptConnection();
	cout<<"LISTENER: accepted socket2"<<endl;
	cout<<"LISTENER FROM SOCKET1: "<<socket1->readLine();
	cout<<"LISTENER FROM SOCKET2: "<<socket2->readLine();
	delete listener;
	delete socket1;
	delete socket2;
}

void socketThread1(){
	Socket* socket = new Socket("127.0.0.1",9999);
	cout<<"SOCKET1: connected to host"<<endl;
	string s = "hello from socket1\n";
	socket->write(s.c_str(), s.length());
	delete socket;
}

void socketThread2(){
	Socket* socket = new Socket("127.0.0.1",9999);
	cout<<"SOCKET2: connected to host"<<endl;
	string s = "hello from socket2\n";
	socket->write(s.c_str(), s.length());
	delete socket;
}

int main(){
	thread t1 (listenerThread);
	usleep(100000);
	thread t2 (socketThread1);
	usleep(100000);
	thread t3 (socketThread2);
	t1.join();
	t2.join();
	t3.join();
}
