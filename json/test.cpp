#include <iostream>

#include "Json.h"

using namespace std;

int main(){
	JsonObject* jsonObj = new JsonObject(); // create an empty json obj
	string* str1 = new string("value1"); // create a string pointer dynamically
	string* str2 = new string("value2"); // create a string pointer dynamically
	jsonObj->add("key1", str1); // test add string function of JsonObject
	jsonObj->add("key2", str2);
	try{
		jsonObj->get("key3"); // test get function of JsonObject when the key does not exist
	}catch(std::string errMsg){
		cerr<<"ERROR: "<<errMsg<<endl;
	}
	try{
		jsonObj->remove("key3"); // test remove function of JsonObject when the key does not exist
	}catch(std::string errMsg){
		cerr<<"ERROR: "<<errMsg<<endl;
	}
	// test for nested json object
	JsonObject* jsonObj2 = new JsonObject();
	string* str3 = new string("value3.1"); 
	jsonObj2->add("key3.1", str3);
	jsonObj->add("key3", jsonObj2);
	JsonArray* jsonArr = new JsonArray();
	string* str4 = new string("value4");
	string* str5 = new string("value5");
	jsonArr->add(str3);
	jsonArr->add(str4);
	jsonObj->add("key4",jsonArr);
	cout<<*jsonObj<<endl; // test overloading operator<< of JsonObject
}
