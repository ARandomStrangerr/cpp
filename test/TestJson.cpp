#include "../Json.h"
#include <stdexcept>

using namespace std;

int main(){
	std::string* str = new string("{\"var1\":\"arg1\",\"var2\":{\"var2.1\":\"arg2.1\"},\"var3\":[\"a1\",\"a2\"]}");
	JsonObject* jsonObj = JsonObject::parse(str);
	cout<<*(jsonObj->get("var3")->getAsJsonArray()->get(0)->getAsString())<<endl;
	cout<<*(jsonObj->get("var2")->getAsJsonObject()->get("var2.1")->getAsString())<<endl;
}
