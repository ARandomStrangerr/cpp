#include "JSON.h"
#include <stdexcept>
#include <string>
#include <vector>

Primitive::Primitive(std::string str){
	this->str = *(new std::string(str));
	this->type = Type::STRING;
}

Primitive::Primitive(std::string* strPtr){
	this->str = *(new std::string(*strPtr));
	this->type = Type::STRING;
}

Primitive::Primitive(double num){
	this->num = num;
	this->type = Type::NUMBER;
}

Primitive::Primitive(double* numPtr){
	this->num = *(new double(*numPtr));
	this->type = Type::NUMBER;
}

Primitive::Primitive(bool boo){
	this->boo = boo;
	this->type = Type::BOOLEAN;
}

Primitive::Primitive(bool* booPtr){
	this->boo = *(new bool(*booPtr));
	this->type = Type::BOOLEAN;
}

Primitive::Primitive(Object obj){
	this->objPtr = &obj;
	this->type = Type::OBJECT;
}

Primitive::Primitive(Object* objPtr){
	this->objPtr = objPtr;
	this->type = Type::OBJECT;
}
Primitive::Primitive(Array arr){
	this->arrPtr = &arr;
	this->type = Type::ARRAY;
}

Primitive::Primitive(Array* arrPtr){
	this->arrPtr = arrPtr;
	this->type = Type::ARRAY;
}

Primitive::~Primitive(){
	// switch (type) {
	// 	case Type::STRING:
	// 		delete &str;
	// 		break;
	// 	case Type::NUMBER:
	// 		delete &num;
	// 		break;
	// 	case Type::BOOLEAN:
	// 		delete &boo;
	// 		break;
	// 	case Type::ARRAY:
	// 		delete arrPtr;
	// 		break;
	// 	case Type::OBJECT:
	// 		delete objPtr;
	// 		break;
	// }
}

std::string Primitive::getStr(){
	if (type == Type::STRING) return this->str;
	throw std::runtime_error("Not string data type");
}

double Primitive::getNum(){
	if (type == Type::NUMBER) return this->num;
	throw std::runtime_error("Not number data type");
}

bool Primitive::getBool() {
	if (type == Type::BOOLEAN) return this->boo;
	throw std::runtime_error("Not boolean type");
}

Object Primitive::getObj() {
	if (type == Type::OBJECT) return *(this->objPtr);
	throw std::runtime_error("Not object type");
}

Array Primitive::getArr() {
	if (type == Type::ARRAY) return *(this->arrPtr);
	throw std::runtime_error("Not array type");
}

std::ostream& operator<< (std::ostream& os, const Primitive& primitive){
	switch (primitive.type){
		case Type::STRING:
			os << '"' << primitive.str << '"';
			break;
		case Type::NUMBER:
			os << primitive.num;
			break;
		case Type::BOOLEAN:
			if (primitive.boo) os << "true";
			else os << "false";
			break;
		case Type::OBJECT:
			os << *(primitive.objPtr);
			break;
		case Type::ARRAY:
			os << *(primitive.arrPtr);
			break;
	}
	return os;
}

Object::Object(){
	map = *(new std::unordered_map<std::string, Primitive*>());
}

Primitive Object::get(std::string key){
	if (map.find(key) != map.end()) return map[key];
	throw std::runtime_error("the key does not exists");
}

void Object::remove(std::string key){
	if (map.find(key) != map.end()){
		delete map[key];
		map.erase(key);
	}
}

void Object::set(std::string key,std::string value){
	Primitive* primitive = new Primitive(value);
	map[key] = primitive;
}

void Object::set(std::string key,double value){
	Primitive* primitive = new Primitive(value);
	map[key] = primitive;
}

void Object::set(std::string key,bool value){
	Primitive* primitive = new Primitive(value);
	map[key] = primitive;
}

void Object::set(std::string key, Object value){
	Primitive* primitive = new Primitive(value);
	map[key] = primitive;
}

void Object::set(std::string key, Array value){
	Primitive* primitive = new Primitive(value);
	map[key] = primitive;
}

Object Object::parse(const std::string& str){
	int startValue = 0;
	return Parse::getObject(startValue, str);
}

std::ostream& operator<< (std::ostream& os, const Object& obj){
	os << "{\n";
	for (const auto& itr : obj.map){
		os << '"' << itr.first << '"' << ":" << *(itr.second)<< "\n";
	}
	os << "}" << std::endl;
	return os;
}

/**
support method to get the key of key-value pair.
the return value is not allocated dynamically
*/
std::string Parse::getKey(int& startIndex, const std::string& str){
	for (int endIndex = startIndex; endIndex < str.length(); endIndex++){
		if (str[endIndex] == ':'){ // key stroke seprate between key and value
			std::string returnValue = str.substr(startIndex, endIndex - startIndex); // obtain the substring
			// beautify the string
			returnValue.erase(0, returnValue.find_first_not_of(" "));
			returnValue.erase(returnValue.find_last_not_of(" ")+1);
			// if there is quote at 2 ends, remove it
			if (returnValue[0] == '"') returnValue.erase(0,1);
			if (returnValue[returnValue.length() -1] == '"') returnValue.erase(returnValue.length()-1);
			// regex checking the syntax of the variable name
			std::regex pattern("^[a-zA-Z_][a-zA-Z0-9_]*$");
			if (!std::regex_search(returnValue, pattern)) throw std::runtime_error("variable name is invalid at " + std::to_string(startIndex));
			startIndex = endIndex;
			return returnValue;
		}
	}
	throw std::runtime_error("missing :");
}

std::string Parse::getString(int& startIndex, const std::string& str){
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

double Parse::getNumber(int& startIndex, const std::string& str){
	std::string tempVal;
	for (int endIndex = startIndex; endIndex < str.length(); endIndex++){
		switch (str[endIndex]) {
			case ',': // cases that the current character signify the end of the variable
			case ']':
			case '}':
				tempVal = str.substr(startIndex, endIndex - startIndex);
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
			case '.':
				break;
			default: // case that the current character is not a number or terminate character
				throw std::runtime_error("not a number at character " + std::to_string(endIndex));
				break;
		}
	}
	throw std::runtime_error("missing terminate character");
	return 0;
}

/**
support method to get the boolean value of key-value pair
*/
bool Parse::getBoolean(int& startIndex, const std::string& str){
	std::string evaluateStr = str.substr(startIndex, 4);
	startIndex+=4;
	if (evaluateStr == "true") return 1;
	else if (evaluateStr == "fase") return 0;
	else throw std::runtime_error("invalid value at " + std::to_string(startIndex - 4));
}

/**
support function to get object value
*/
Object Parse::getObject(int& startIndex, const std::string& str){
	Object obj = *(new Object()); // dynatmically allocate object
	Primitive* primitive;
	std::string key = "";
	int openBrace = 0; // count the braces
	for (int endIndex = startIndex; endIndex < str.length(); endIndex++){
		switch(str[endIndex]){
			case '{': // either this is the very beginning or start of a Object value
				openBrace++; // increase the count to detect when to stop
				if (key.empty()){ // if the key is empty then it is the begining of the Object
					endIndex++; // skip the '{' char
					key = getKey(endIndex, str);
				} else { // if key is recoreded then it is begining of value
					primitive = new Primitive(getObject(endIndex, str));
					obj.set(key, primitive);
					key = "";
				}
				break;
			case '}': // possibly the terminate character of this function
				openBrace--;
				if (openBrace == 0){ // it is the terminate character of this function
					startIndex = endIndex;
					return obj;
				}
				else if (openBrace < 0) // excessive amount of '}' happened
					throw std::runtime_error("incorrect } at " + std::to_string(endIndex));
				break;
			case '[': // start of Array
				break;
			case 't': // possible start of bolean true value
			case 'f': // possible start of bolean false value
				primitive = new Primitive(getBoolean(endIndex, str));
				obj.set(key, primitive);
				break;
			case '"': // possible start of of string value
				primitive = new Primitive(getString(endIndex,str));
				obj.set(key, primitive);
				break;
			case ',': // separator between each key:value pair
				key = getKey(endIndex, str);
				break;
			case ' ': // ignore the space character
				break;
			default: // character is out of line
				throw std::runtime_error("invalid character at " + std::to_string(endIndex));
				break;
		}
	}
	throw std::runtime_error("Missing terminate character ");
}

/**
support function to get array returnValue
*/
Array getArray(int& startIndex, const std::string& str){
	Array* arr = new Array();
	return *arr;
}

Array::Array(){
	vec = new std::vector<Primitive*>();
}
