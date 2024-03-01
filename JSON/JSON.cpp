#include "JSON.h"
#include <stdexcept>

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
			break;
		case Type::NUMBER:
			delete &num;
			break;
		case Type::BOOLEAN:
			delete &boo;
			break;
		case Type::ARRAY:
			delete arrPtr;
			break;
		case Type::OBJECT:
			delete objPtr;
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
	if (map.find(key) != map.end()) return map[key];
	throw std::runtime_error("the key does not exists");
}

void Object::remove(std::string key){
	map.erase(key);
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

std::ostream& operator<< (std::ostream& os, const Object& obj){
	os << "{\n";
	for (const auto& itr : obj.map){
		os << '"' << itr.first << '"' << ":" << *(itr.second)<< "\n";
	}
	os << "}" << std::endl;
	return os;
}

std::string getKey(int& startIndex, const std::string& str){
	for (int endIndex = startIndex; endIndex < str.length(); endIndex++){
		switch (str[endIndex]) {
			case ':': // key stroke seprate between key and value
				std::string returnValue = str.substr(startIndex, endIndex - startIndex); // obtain the substring
				// beautify the string
				returnValue.erase(0, returnValue.find_first_not_of(" "));
				returnValue.erase(returnValue.find_last_not_of(" "))
				return returnValue
		}
	}
	throw std::runtime_error("missing :");
}

Object Object::parse(const std::string&){

}
