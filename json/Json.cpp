#include "Json.h"

std::string* JsonPrimitive::getStr(){
	return this->strPtr;
}

JsonObject* JsonPrimitive::getJsonObj(){
	if (this->jsonObjPtr) return this->jsonObjPtr;
	throw ("This object is not Json Object");
}

JsonArray* JsonPrimitive::getJsonArr(){
	if (this->jsonArrPtr) return this->jsonArrPtr;
	throw ("This object is not Json Array");
}

std::string JsonPrimitive::beautifyDisplay(int tab){
	if (this->strPtr) return "\""+*this->strPtr+"\"";
	if (this->jsonArrPtr) return this->jsonArrPtr->beautifyDisplay(tab);
	else return this->jsonObjPtr->beautifyDisplay(tab);
}

JsonObject::JsonObject(){
	mapPtr = new std::map<std::string, JsonPrimitive*>();
}

void JsonObject::add(std::string key, std::string* value){
	JsonPrimitive* obj = new JsonPrimitive();
	obj->strPtr = value;
	(*this->mapPtr)[key] = obj;
}

void JsonObject::add(std::string key, JsonObject* value){
	JsonPrimitive* obj = new JsonPrimitive();
	obj->jsonObjPtr = value;
	(*this->mapPtr)[key] = obj;
}

void JsonObject::add(std::string key, JsonArray* value){
	JsonPrimitive* obj = new JsonPrimitive();
	obj->jsonArrPtr = value;
	(*this->mapPtr)[key] = obj;
}

JsonPrimitive* JsonObject::get(std::string key){
	if (this->mapPtr->count(key)) return (*this->mapPtr)[key];
	throw ("There is no key associated with the key: "+key);
}

JsonPrimitive* JsonObject::remove(std::string key){
	if (this->mapPtr->count(key)) {
		JsonPrimitive* i = (*this->mapPtr)[key];
		return i;
	}
	throw ("There is no key associated with the key: "+key);
}

std::ostream& operator<<(std::ostream& os, JsonObject& obj){
	os << obj.beautifyDisplay(1);
	return os;
}

std::string JsonObject::beautifyDisplay(int tab){
	std::string prepend = "";
	for (int i = 0; i < tab; i++)prepend+="\t";
	std::string str = "";
	for(std::map<std::string, JsonPrimitive*>::iterator itr = this->mapPtr->begin(); itr != this->mapPtr->end(); itr++){
		std::cout<<itr->first<<std::endl;
		if (str.size()!=0)str+=",\n";
		str+=prepend+"\"";
		str+=itr->first;
		str+="\":";
		str+=itr->second->beautifyDisplay(tab+1);
	}
	str.insert(0,"{\n");
	str+="\n";
	for(int i=0;i<tab-1;i++)str+="\t";
	str+="}";
	return str;
}

JsonArray::JsonArray(){
	this->vec = new std::vector<JsonPrimitive*>();
}

void JsonArray::add(std::string* strPtr){
	JsonPrimitive* obj = new JsonPrimitive();
	obj->strPtr = strPtr;
	this->vec->push_back(obj);
}

void JsonArray::add(JsonObject* jsonObjPtr){
	JsonPrimitive* obj = new JsonPrimitive();
	obj->jsonObjPtr = jsonObjPtr;
	this->vec->push_back(obj);
}

void JsonArray::add(JsonArray* jsonArrPtr){
	JsonPrimitive* obj = new JsonPrimitive();
	obj->jsonArrPtr = jsonArrPtr;
	this->vec->push_back(obj);
}

JsonPrimitive* JsonArray::get(int index){
	if (index>=0&&index<this->vec->size()) return (*this->vec)[index];
	throw ("index is out of bound");
}

JsonPrimitive* JsonArray::remove(int index){
	if (index>=0&&index<this->vec->size()){
		JsonPrimitive* obj = (*this->vec)[index];
		this->vec->erase(this->vec->begin()+index);
		return obj;
	}
	throw ("index is out of bound");
}

std::ostream& operator<<(std::ostream& os, JsonArray& obj){
	os<<obj.beautifyDisplay(1);
	return os;
}

std::string JsonArray::beautifyDisplay(int tab){
	std::string str = "";
	std::string prepend = "";
	for(int i=0;i<tab;i++)prepend+="\t";
	for(std::vector<JsonPrimitive*>::iterator itr=this->vec->begin();itr!=vec->end();itr++){
		if (str.length()!=0) str+=",\n";
		str+=prepend+(*itr)->beautifyDisplay(tab+1);
	}
	str.insert(0,"[\n");
	str+="\n";
	for(int i=0;i<tab-1;i++)str+="\t";
	str+="]";
	return str;
}
