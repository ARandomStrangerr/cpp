#include "HttpSession.h"
#include <thread>
#include <iostream>

using namespace std;
void testListener() {
	Listener* listener = new Listener(9999,2);
	listener->accept();
	std::cout<<"accept a socktet"<<std::endl;
}

void testSocket() {
	std::string addr = "127.0.0.1";
	Socket* socket = new Socket(addr, 9999);
}

void testJsonObject() {
	JsonObject obj;
	obj.add("strValue", (std::string) "hello1234");
	obj.add("numericValue", (double) 12345);
	obj.add("booleanValue", (bool) 1);
	JsonArray arr;
	arr.add((std::string) "str val");
	arr.add((double) 1996);
	arr.add((bool) 0);
	JsonObject obj2;
	obj2.add("jsonObject", &obj);
	obj2.add("jsonArr", &arr);
	cout<<obj2<<endl;
}

void testParseJsonObject(){
	std::string blob =
		"{\"strValue\":\"hello1234\",\"numericValue\":12345, \"booleanValue\":true, \"nestedJsonValue\":{\"nestedString\": \"nested string\"}, \"jsonArray\":[12345, \"a string\", true]}";
	JsonObject obj = JsonObject::parse(blob);
	cout<<obj<<endl;
}

void testHttpSession(){
	HttpSession session (9999, 2);
	session.start();
}

int main() {
	testHttpSession();
	return 0;
}
