#include "JSON.h"
#include <iostream>

using namespace std;

int main() {
	Object* obj = new Object();
	obj->set("value1", 10.0);
	obj->set("value2", std::string("Hello"));
	obj->set("value3", true);
	cout<<*obj<<endl;
}
