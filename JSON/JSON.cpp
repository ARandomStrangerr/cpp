#include "JSON.h"
#include <stdexcept>
#include <string>

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
	switch (type) {
		case Type::STRING:
			delete &str;
			str = nullptr;
			break;
		case Type::NUMBER:
			delete &num;
			break;
		case Type::BOOLEAN:
			delete &boo;
			break;
		case Type::ARRAY:
			delete arrPtr;
			arrPtr = nullptr;
			break;
		case Type::OBJECT:
			delete objPtr;
			objPtr = nullptr;
			break;
	}
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
	if (map.find(key) != map.end()) return *map[key];
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
the return value is not allocated dynamically.
parse the string until see ':'
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
			std::cout << returnValue << std::endl;
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

bool Parse::getBoolean(int& startIndex, const std::string& str){
	if (str.substr(startIndex, 5) == "false") {
		startIndex+=4;
		return 0;
	} else if (str.substr(startIndex, 4) == "true") {
		startIndex+=3;
		return 1;
	} else throw std::runtime_error("Incorrect tokken of possible boolean value at: " + std::to_string(startIndex));
}

/**
process character by character.
this ensure that this algorithm is running in O(n)
to run this funciton, it must start with '{', hence the first character is skip.
then right after that, it must be follow by a key.
character:
space character - ignore
double quote - string value
number - number value
open curly brace - object value
close culty brace - termination of this function
open square brace - array value
character 't' or 'f' - boolean value
character comma - separate character
reason:
the key always follow by ':' and then value.
the key and ':' is handled by Parse::getKey(int& startIndex, const std::string& str); hence, parse value, check if key exists. after parse value, the key should be reset.
when see ',' it must be follow by a key; hence call Parse::getKey(int& startIndex, const std::string& str)
when the case 2 commas consecutive, (, , "key" : "value") the Parse::getKey(int& startIndex, const std::string& str) function will yield error since it is not suitable to be variable name
the case when there is no ':', ("key" "value"), the Parse::getKey(int& startIndex, const std::string& str) will yeild error since ':' is the terminate character of that function
(it might be in efficient since it make the Parse::getKey(int& startIndex, const std::string& str) to O(n))
the case where there is no value, ("key": ,) then the key will not be reset, when it see a ',' error will be throw
the case where there is no key, (, :"value"), Parse::getKey(int& startIndex, const std::string& str) will throw an error that it is not suitable to be variable name.
the case where
any other character will throw an error, 2 colon ("key"::"value")
*/
Object Parse::getObject(int& startIndex, const std::string& str){
	int endIndex = startIndex+1;
	std::string key = getKey(endIndex, str); // the start of an Object should be a key.
	endIndex++; // after above funciton, it stops at ':'
	Object obj = *(new Object()); // dynamically allocate object
	for (; endIndex < str.length(); endIndex++) { // now the string should start at ':'
		switch(str[endIndex]) {
			case '{': // start of a Object value
				if (key.empty()) throw std::runtime_error("Missing key at: " + std::to_string(endIndex));
				obj.set(key, Parse::getObject(endIndex, str));
				key = "";
				break;
			case '}': // terminate character of this function
				if (key.empty()) return obj;
				throw std::runtime_error("Missing value at: " + std::to_string(endIndex));
				break;
			case '[': // start of Array
				if (key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				obj.set(key, Parse::getArray(endIndex, str));
				key = "";
				break;
			case 't': // possible start of bolean true value
			case 'f': // possible start of bolean false value
				if (key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				obj.set(key, getBoolean(endIndex, str));
				key = "";
				break;
			case '"': // possible start of of string value
				if (key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				obj.set(key, getString(endIndex,str));
				key = "";
				break;
			case ',': // separator between each key:value pair
				if (!key.empty()) throw std::runtime_error("Incorrect tokken at: " + std::to_string(endIndex));
				endIndex++; // the current character is ',' the get key will not process this.
				key = getKey(endIndex, str);
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
				obj.set(key, Parse::getNumber(endIndex, str));
				key = "";
				break;
			default: // character is invalid
				throw std::runtime_error("invalid character at " + std::to_string(endIndex));
				break;
		}
	}
	throw std::runtime_error("Missing terminate character ");
}

/**
parse array is way easier compare to Object relation
isFoundValue to check that value is found, prevent error when 2 comma in consecutive or comma righ before ']'
when found any following character, isFoundValue = true -> throw an error
when '"' parse string
when number, parse number.
when 't' or 'f', parse boolean
when '{' parse Object
when '[' parse Array
when ']' it is the terminate of this function and
when ',' isFoundValue set to false. if isFoundValue = false, then two commas are consecutive
*/
Array Parse::getArray(int& startIndex, const std::string& str){
	bool isFoundValue = 0; // checking if a value is found
	Array arr = *(new Array());
	for (int endIndex = startIndex+1; endIndex < str.length(); endIndex++){ // advance the character by 1 since to enter this stage, it start with '['
		switch(str[endIndex]){
			case '[': // case of a nested array
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr.insert(Parse::getArray(endIndex,str));
				isFoundValue = 1; // value is found
				break;
			case ']': // end of this array
				if (isFoundValue) {
					startIndex = endIndex;
					return arr; // teminate charcater should be meet before a value
				}
				else throw std::runtime_error("Missing value before terminate character at " + std::to_string(endIndex));
				break;
			case '{': // case of an object
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr.insert(Parse::getArray(endIndex, str));
				isFoundValue = 1;
				break;
			case '"': // start of a string
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr.insert(Parse::getString(endIndex, str));
				isFoundValue = 1; // value is found
				break;
			case 't': // case that it is a boolean
			case 'f':
				if (isFoundValue) throw std::runtime_error("Missing separate character at " + std::to_string(endIndex)); // found 2 consecutive values without a separate character
				arr.insert(Parse::getBoolean(endIndex, str));
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
				arr.insert(Parse::getNumber(endIndex, str));
				isFoundValue = 1; // value is found
				break;
			case ' ': // ignore space character
				break;
			case ',': // case of separate character
				if (isFoundValue) isFoundValue = 0; // case that the value
				else throw std::runtime_error("No value is found before a comma" + std::to_string(endIndex));
			default: // any other character is an error
				throw std::runtime_error("unknwon tokken " + std::to_string(endIndex));
		}
	}
	throw std::runtime_error("Missing terminate character");
}

Array::Array(){
	vec = new std::vector<Primitive*>();
}

void Array::insert(std::string str){
	Primitive* primitive = new Primitive(str);
	this->vec->push_back(primitive);
}

void Array::insert(double num){
	Primitive* primitive = new Primitive(num);
	this->vec->push_back(primitive);
}

void Array::insert(bool boolean){
	Primitive* primitive = new Primitive(boolean);
	this->vec->push_back(primitive);
}

void Array::insert(Object obj){
	Primitive* primitive = new Primitive(obj);
	this->vec->push_back(primitive);
}

void Array::insert(Array arr){
	Primitive* primitive = new Primitive(arr);
	this->vec->push_back(primitive);
}


Array Array::parse(const std::string& str){
	int startIndex = 0;
	return Parse::getArray(startIndex, str);
}

Array::~Array(){
	for(auto item : *(this->vec)) delete item;
	this->vec->clear();
	delete vec;
	vec = nullptr;
}
