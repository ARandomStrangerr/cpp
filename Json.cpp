#include <string>
#include "Json.h"

JsonPrimitive::JsonPrimitive(std::string str){
	this -> str = &str;
}

JsonPrimitive::JsonPrimitive(JsonObject obj){
	this -> jsonObj = &obj;
}

JsonPrimitive::JsonPrimitive(JsonArray arr){
	this -> jsonArr = &arr;
}

JsonObject JsonPrimitive::getAsJsonObject(){
	if (jsonObj) return *(this->jsonObj);
	else if (str) {
		JsonObject *obj = new JsonObject(*(this->str));
		this->jsonObj = obj;
		return *(this->jsonObj);
	} else throw std::runtime_error("");
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
		m[key] = new JsonPrimitive(value);
	}
}

void verification(std::string str){
	std::string key,value,temp;
	bool quote = 0,
		 colon = 0;
	int curlyBrace = 0,
		squareBrace = 0;
	for (int index = 0; index < str.size(); index++){
		char currentChar = str[index];
		switch (currentChar){ // examining the current character
			/*
			 * can be open or close quote
			 * signify the end of a string
			 * this string can be key or value of type string
			 */
			case '"':
				if (quote) // case when this quote is close
					quote = 0;
				else { // case when this quote is open
					quote = 1;
					/*
					 * this error occurs when the temp string is not empty
					 * since after a reset character : , [ ] { } temp will be reset.
					 * the existence of temp proof that those reset characters have not appeared
					 * hence, Json is incorrect format
					 */
					if (!temp.empty()) throw std::invalid_argument("incorrect character at " + std::to_string(index));
				}
				break;
			// key stroke to separate between key-value pair
			case ':':
				if (!quote) { // check that this colon is not inside a string
					if (colon) throw std::runtime_error("incorrect character at " + std::to_string(index)); // this error occurs when there is a colon appear in the key - value pair already.
					colon = 1; // register that close has been seen for this key - value pair
					key = temp.c_str(); // copy the temp string into the key
					temp = ""; // reset the temp variable
				}
				break;
			// separate between current key-value pair and the next key-value pair
			case ',':
				if (!quote) { // check if this comma is outside a string
					colon = 0;
					value = temp.c_str();
					temp = "";
					std::cout<<key<<"|"<<value<<std::endl;
				}
				break;
			case '{':
				if (!quote) curlyBrace++;
				break;
			case '}':
				if (!quote){
					curlyBrace--;
					colon = 0;
					value = temp.c_str();
					temp = "";
					std::cout<<key<<"|"<<value<<std::endl;
				}
				break;
			case '[':
				if (!quote) squareBrace++;
				break;
			case ']':
				if (!quote) squareBrace--;
			// default character
			default:
				if (quote) temp += currentChar; // append the current character into string if inside quote
				else throw std::runtime_error("incorrect character at " + std::to_string(index));
				break;
		}
	}
}
