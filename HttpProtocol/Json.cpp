#include "Json.h"

//JsonPrimitive functions
JsonPrimitive::JsonPrimitive(){}
std::string JsonPrimitive::getAsString(){
	if (this->type == PrimitiveType::STRING) return strVal; 
	throw new std::runtime_error("not a string");
}
bool JsonPrimitive::getAsBoolean(){
	if (this->type == PrimitiveType::BOOLEAN) return boolVal;
	throw new std::runtime_error("not a boolean");
}
double JsonPrimitive::getAsNumber(){
	if (this->type == PrimitiveType::NUMBER) return numVal;
	throw new std::runtime_error("not a number");
}
JsonObject& JsonPrimitive::getAsObj(){
	if (this->type == PrimitiveType::JSON_OBJECT) return *jsonObjPtr;
	throw new std::runtime_error("not a json object");
}
JsonArray& JsonPrimitive::getAsArr(){
	if (this->type == PrimitiveType::JSON_ARRAY) return *jsonArrPtr;
	throw new std::runtime_error("not a json array");
}
std::string JsonPrimitive::beautifyDisplay(int tab) const {
	switch (type) {
		case PrimitiveType::STRING:
			return "\""+strVal+"\"";
		case PrimitiveType::BOOLEAN:
			return boolVal?"true":"false";
		case PrimitiveType::NUMBER:
			return std::to_string(numVal);
		case PrimitiveType::JSON_ARRAY:
			return jsonArrPtr->beautifyDisplay(tab);
		case PrimitiveType::JSON_OBJECT:
			return jsonObjPtr->beautifyDisplay(tab);
		default:
			throw std::runtime_error("nope");
	}
}

// Json Object functions
int JsonObject::size(){
	return this->map.size();
}
JsonPrimitive& JsonObject::get(std::string key){
	return map[key]; // will throw an error std::out_of_range when the key does not exist in the collection, why write more codes to check when it can be done in 1 line? :)
}
JsonPrimitive& JsonObject::remove(std::string key){
	for (auto it = map.begin(); it != map.end(); it++){ // well, you have to iterate through the collection to get the position of the iterator anyway.
		if (it->first == key){
			JsonPrimitive& returnValue = it->second;
			this->map.erase(it);
			return returnValue;
		}
	}
	throw std::out_of_range("the given key does not exist");
}
void JsonObject::add(std::string key, std::string value) {
	this->map[key] = JsonPrimitive(); // scope create the variable so that when map is out of scope, it will automatically reclaim the memory
	this->map[key].strVal = value;
	this->map[key].type = PrimitiveType::STRING;
}
void JsonObject::add(std::string key, bool value) {
	this->map[key] = JsonPrimitive();
	this->map[key].boolVal = value;
	this->map[key].type = PrimitiveType::BOOLEAN;
}
void JsonObject::add(std::string key, double value) {
	this->map[key] = JsonPrimitive();
	this->map[key].numVal = value;
	this->map[key].type = PrimitiveType::NUMBER;
}
void JsonObject::add(std::string key, JsonObject* value){
	this->map[key] = JsonPrimitive();
	this->map[key].jsonObjPtr = value;
	this->map[key].type = PrimitiveType::JSON_OBJECT;
}
void JsonObject::add(std::string key, JsonArray* value) {
	this->map[key] = JsonPrimitive();
	this->map[key].jsonArrPtr = value;
	this->map[key].type = PrimitiveType::JSON_ARRAY;
}
std::string JsonObject::beautifyDisplay(int tab) const {
	std::string str = "{\n";
	for (auto itr = map.begin(); itr!= map.end(); itr++){
		for (int i = 0; i < tab+1; i++) str+="\t";
		str+= "\"" + itr->first + "\":"+ itr->second.beautifyDisplay(tab+1)+"\n";
	}
	for (int i = 0; i < tab; i++) str+="\t";
	str+="}";
	return str;
}
std::ostream& operator<<(std::ostream& os, const JsonObject& obj){
	os << obj.beautifyDisplay(0);
	return os;
}
//JsonArray functions
int JsonArray::length(){
	return vec.size();
}
JsonPrimitive& JsonArray::get(int index){
	if (index < 0 || index >= vec.size()) throw new std::out_of_range("index out of bound");
	return vec[index];
}
JsonPrimitive& JsonArray::remove(int index){
	if (index < 0 || index >= vec.size()) throw new std::out_of_range("index out of bound");
	auto itr = vec.begin();
	for (int i = 0; i != index; i++, itr++);
	return *itr;
}
void JsonArray::add(double value){
	JsonPrimitive jp;
	jp.type = PrimitiveType::NUMBER;
	jp.numVal = value;
	vec.push_back(jp);
}
void JsonArray::add(std::string value){
	JsonPrimitive jp;
	jp.type = PrimitiveType::STRING;
	jp.strVal = value;
	vec.push_back(jp);
}
void JsonArray::add(bool value){
	JsonPrimitive jp;
	jp.type = PrimitiveType::BOOLEAN;
	jp.boolVal = value;
	vec.push_back(jp);
}
void JsonArray::add(JsonArray* value) {
	JsonPrimitive jp;
	jp.type = PrimitiveType::JSON_ARRAY;
	jp.jsonArrPtr = value;
	vec.push_back(jp);
}
void JsonArray::add(JsonObject* value) {
	JsonPrimitive jp;
	jp.type = PrimitiveType::JSON_OBJECT;
	jp.jsonObjPtr = value;
	vec.push_back(jp);
}
std::string JsonArray::beautifyDisplay(int tab) const {
	std::string returnVal = "[\n";
	for (auto itr = vec.begin(); itr != vec.end(); itr++){
		for (int i = 0; i < tab+1; i++) returnVal+="\t";
		returnVal+=(*itr).beautifyDisplay(tab+1)+"\n";
	}
	for (int i = 0; i < tab; i++) returnVal += "\t";
	returnVal+= "]";
	return returnVal;
}
std::ostream& operator<< (std::ostream& os, const JsonArray& obj){
	os << obj.beautifyDisplay(0);
	return os;
}

// parse functions
std::string getKey(const std::string& blob, int& startIndex){
	for (int endIndex = startIndex + 1; endIndex < blob.length(); endIndex++){
		if (blob[endIndex] == ':'){
			std::string str = blob.substr(startIndex, endIndex-startIndex);
			str.erase(0, str.find_first_not_of(" "));
			str.erase(str.find_last_not_of(" ")+1);
			if (str[0] == '"') str.erase(0,1);
			if (str[str.length()-1] == '"') str.erase(str.length()-1);
			std::regex pattern("^[a-zA-Z_][a-zA-Z0-9_]*$");
			if (!std::regex_search(str, pattern)) throw std::runtime_error("variable name is invalid at " + std::to_string(startIndex));
			startIndex = endIndex; 
			return str;
		}
	}
	throw std::out_of_range("missing colon for key sart at "+ std::to_string(startIndex));
}

double getNumber(const std::string& blob, int& startIndex){
	std::string tempVal;
	for (int endIndex = startIndex; endIndex < blob.length(); endIndex++){
		switch (blob[endIndex]) {
			case ',': // cases that the current character signify the end of the variable
			case ']':
			case '}':
				tempVal = blob.substr(startIndex, endIndex - startIndex);
				startIndex = endIndex-1;
				return std::stod(tempVal);
				break;
			case '0': // cases that the current character is a number
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.': // this can be here due to the when this function is called by getObject or getArray, the first character is a number already
				break;
			default: // case that the current character is not a number or terminate character
				throw std::runtime_error("not a number at character " + std::to_string(endIndex));
				break;
		}
	}
	throw std::runtime_error("missing terminate character");
	return 0;
}

bool getBoolean(const std::string& blob, int& startIndex){
	if (blob.substr(startIndex, 5) == "false") {
		startIndex+=4;
		return 0;
	} else if (blob.substr(startIndex, 4) == "true") {
		startIndex+=3;
		return 1;
	} else throw std::runtime_error("Incorrect tokken of possible boolean value at: " + std::to_string(startIndex));
}

std::string getString(const std::string& str, int& startIndex){
	for (int endIndex = startIndex + 1; endIndex < str.length(); endIndex++){ // advance the character by 1 since going into this state, it start with "
		if (str[endIndex] == 34 && str[endIndex-1] != 92) { // case when it is quote and the previous character is not except character
			std::string returnValue = str.substr(startIndex+1, endIndex-startIndex-1); // copy the string, exclude the 2 quotes on 2 sides
			// trim the string
			returnValue.erase(0, returnValue.find_first_not_of(" "));
			returnValue.erase(returnValue.find_last_not_of(" ")+1);
			startIndex = endIndex;
			return returnValue;
		}
	}
	throw std::runtime_error("missing the termination '\"' for the string data start at " + std::to_string(startIndex));
}

JsonObject* getJsonObject(const std::string&, int&);
JsonArray* getJsonArray(const std::string&, int&);

JsonObject* getJsonObject(const std::string& str, int& startIndex){
	int endIndex = startIndex+1;
	std::string key = getKey(str, endIndex); // the start of an Object should be a key.
	endIndex++; // after above funciton, it stops at ':'
	JsonObject* obj = new JsonObject(); // dynamically allocate object
	for (; endIndex < str.length(); endIndex++) { // now the string should start at ':'
		switch(str[endIndex]) {
			case '{': // start of a Object value
				if (key.empty()) throw std::runtime_error("Missing key at: " + std::to_string(endIndex));
				obj->add(key, getJsonObject(str, endIndex));
				key = "";
				break;
			case '}': // terminate character of this function
				if (key.empty()) {
					startIndex = endIndex;
					return obj;
				}
				throw std::runtime_error("Missing value at: " + std::to_string(endIndex));
				break;
			case '[': // start of Array
				if (key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				obj->add(key, getJsonArray(str, endIndex));
				key = "";
				break;
			case 't': // possible start of bolean true value
			case 'f': // possible start of bolean false value
				if (key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				obj->add(key, getBoolean(str, endIndex));
				key = "";
				break;
			case '"': // possible start of of string value
				if (key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				obj->add(key, getString(str, endIndex));
				key = "";
				break;
			case ',': // separator between each key:value pair
				if (!key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				endIndex++; // the current character is ',' the get key will not process this.
				key = getKey(str, endIndex);
				break;
			case ' ': // ignore the space character
				break;
			case '0': // case that it is a number
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				obj->add(key, getNumber(str, endIndex));
				key = "";
				break;
			default: // character is invalid
				throw std::runtime_error("invalid character at " + std::to_string(endIndex));
				break;
		}
	}
	throw std::runtime_error("Missing terminate character ");
}

JsonArray* getJsonArray(const std::string& blob, int& startIndex){
	bool isFoundValue = 0; // checking if a value is found
	JsonArray* arr = new JsonArray();
	for (int endIndex = startIndex+1; endIndex < blob.length(); endIndex++){ // advance the character by 1 since to enter this stage, it start with '['
		switch(blob[endIndex]){
			case '[': // case of a nested array
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr->add(getJsonArray(blob, endIndex));
				isFoundValue = 1; // value is found
				break;
			case ']': // end of this array
				if (isFoundValue) {
					startIndex = endIndex;
					return arr;
				}
				else throw std::runtime_error("Missing value before terminate character at " + std::to_string(endIndex));
				break;
			case '{': // case of an object
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr->add(getJsonObject(blob, endIndex));
				isFoundValue = 1;
				break;
			case '"': // start of a string
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr->add(getString(blob, endIndex));
				isFoundValue = 1; // value is found
				break;
			case 't': // case that it is a boolean
			case 'f':
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr->add(getBoolean(blob, endIndex));
				isFoundValue = 1; // value is found
				break;
			case '0': // case that it is a number
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr->add(getNumber(blob, endIndex));
				isFoundValue = 1; // value is found
				break;
			case ' ': // ignore space character
				break;
			case ',': // case of separate character
				if (isFoundValue) isFoundValue = 0; // case that the value
				else throw std::runtime_error("No value is found before a comma" + std::to_string(endIndex));
				break;
			default: // any other character is an error
				throw std::runtime_error("unknwon tokken " + std::to_string(endIndex));
		}
	}
	throw std::runtime_error("Missing terminate character");
}

JsonObject& JsonObject::parse(std::string blob){
	int startIndex = 0;
	return *getJsonObject(blob, startIndex);
}

JsonArray& JsonArray::parse(std::string blob){
	int startIndex = 0;
	return *getJsonArray(blob, startIndex);
}
