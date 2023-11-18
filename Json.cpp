#include "Json.h"

JsonPrimitive::JsonPrimitive(std::string str){
	this -> str = new std::string(str);
}

JsonPrimitive::JsonPrimitive(JsonObject obj){
	this -> jsonObj = &obj;
}

std::string JsonPrimitive::getAsString(){
	if (str) return *str;
	throw std::runtime_error("this string is alaready converted into different data type");
}

JsonPrimitive::JsonPrimitive(JsonArray arr){
	this -> jsonArr = &arr;
}

JsonObject JsonPrimitive::getAsJsonObject(){
	if (jsonObj) return *(this->jsonObj); // if the json is already there, return it
	else if (str) { // attemp to convert string into json
		JsonObject *obj = new JsonObject(*(this->str));
		this->jsonObj = obj;
		return *(this->jsonObj);
	} else throw std::runtime_error(""); // error because it is json array
}

int getEndKeyIndex(std::string str, int index) {
	/* 
	 * this method only work when the json format is correct. Averification must be done before calling this
	 * the key will be before colon. 
	 * we capture the string before the key then store it as key.
	 */
	for (int i = index; i < str.size(); i++){
		if (str[i] == ':') return i;
	}
	return -1;
}

int getEndValueIndex(std::string str, int index){
	/* 
	 * this method only work when the json format is correct. A verification must be done before calling this method.
	 * the value will be between a colon and comma
	 * however, comma can occures during a quote (string), nested Json Object, or nested JsonArray
	 * comma out side of quote will be count
	 * comma at level 0 of a json object or array will be count
	 * we need to count the square brace and curly brace.
	 */
	int squareBrace = 0,
		curlyBrace = 0;
	bool quote = 0;
	for (int i = index; i < str.size(); i++){
		char currentChar = str[i];
		switch (currentChar) {
			case '"':
				if (quote) quote = 0;
				else quote = 1;
				break;
			case '{':
				if (!quote) curlyBrace++;
				break;
			case '}':
				if (!quote) curlyBrace--;
				break;
			case '[':
				if (!quote) squareBrace++;
				break;
			case ']':
				if (!quote) squareBrace--;
			case ',':
				if (squareBrace == 0 && curlyBrace == 0) return i;
		}
	}
	return str.size()-1;
}

void strip(std::string &str){
	// remove frontal unwanted character
	int index = 0;
	while (str[index] == '"' || str[index] == ':' || str[index] == ' ')	index++;
	str.erase(0, index);
	// remove posterior unwanted character
	index = str.size()-1;
	while (str[index] == '"' || str[index] == ':' || str[index] == ' ') index--;
	str.erase(index+1, str.size() - index +1); 
}

JsonObject::JsonObject(std::string str){
	for (int index = 1; index < str.size(); index++){
		int endKeyIndex = getEndKeyIndex(str, index);
		std::string key = str.substr(index, endKeyIndex - index);
		strip(key);
		int	endValueIndex = getEndValueIndex(str, endKeyIndex);
		std::string	value = str.substr(endKeyIndex, endValueIndex - endKeyIndex);
		strip(value);
		index = endValueIndex;
		JsonPrimitive* jsonPrimitive = new JsonPrimitive(value);
		m[key] = jsonPrimitive;
	}
}

JsonPrimitive JsonObject::get(std::string key){
	if (m[key]) return *m[key];
	throw std::invalid_argument("the given key does not associated with any value");
}

JsonObject JsonObject::parse(std::string str){
	JsonObject returnObj (str);
	return returnObj;
}

JsonArray::JsonArray(std::string str){
	for (int index = 0; index < str.length(); index++){
		int endValueIndex = getEndKeyIndex(str, index);
		std::string value = str.substr(index, endValueIndex - index);
		strip(value);
		index = endValueIndex;
		JsonPrimitive* jsonPrimitive = new JsonPrimitive(value);
		vec.push_back(jsonPrimitive);
	}
}
