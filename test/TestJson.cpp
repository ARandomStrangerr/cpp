#include "../Json.h"
#include <stdexcept>

using namespace std;

int main(){
	std::string str =  "{\"var1\":\"arg1\",\"var2\":{\"var2.1\":\"arg2.1\"},\"var3\":[\"a1\",\"a2\"]}";
	JsonObject* jsonObj = JsonObject::parse(str); // parse the string into JsonObject
	jsonObj->get("var2")->getAsJsonObject(); // convert the var2 to JsonObject
	try {
		jsonObj->get("var0")->getAsJsonObject(); // this will throw an error due to no keyword associated with var0
	} catch (std::invalid_argument e) {
		std::cout<<e.what()<<std::endl;
	}
	try {
		jsonObj->get("var2")->getAsJsonArray(); // this will throw an error due to the data is Jsonobject, not JsonArray
	} catch (std::runtime_error e) {
		std::cout<<e.what()<<std::endl;
	}
	jsonObj->get("var3")->getAsJsonArray();
}
