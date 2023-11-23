#include <iostream>
#include <stdexcept>
#include <string.h>
#include <map>
#include <stdexcept>

class JsonPrimitive;
class JsonObject;
class JsonArray;

class JsonPrimitive {
	friend class JsonObject;
	friend class JsonArray;
	private:
		/*
		 * only one of the following stage will be exist at a given time
		 * string, numberic will be stored as string
		 * once the string converted either to JsonObject or JsonArray, ther will be no reverse
		 * the default is to record the data as string then convert when methods are called.
		 */
		JsonObject* jsonObj;
		JsonArray* jsonArr;
		std::string* str;
		/**
		 * this JsonPrimitive holds string
		 * pass by pointer to avoid copy of large object
		 * input
		 * 	std::string* the pointer to the String to be stored
		 */
		JsonPrimitive(std::string*);
		/**
		 * this jsonPrimitive holds JsonObject
		 * pass by pointer to avoid copy of large object
		 * input
		 * 	JsonObject* pointer to the JsonObject to be stored
		 */
		JsonPrimitive(JsonObject*);
		/**
		 * this JsonPrimitve holds JsonArray.
		 * pass by pointer to avoid copy of large object
		 * input
		 * 	JsonArray* pointer to JsonArray to be stored
		 */
		JsonPrimitive(JsonArray*);
	public:
		/**
		 * return the stored string stored in this object
		 * return
		 * 	str::string* pointer to a string whic is held by this JsonPrimitive
		 * error
		 * 	std::runtime_error when *str is null
		 */
		std::string* getAsString();
		/**
		 * return exists JsonObject or attempt to convert the stored string into JsonObject
		 * return
		 * 	JsonObject* pointer to a JsonObject which is held by this JsonPrimitive
		 * error
		 * 	std::runtime_error when *str and *jsonObj are null
		 */
		JsonObject* getAsJsonObject();
		/**
		 * return exists JsonArray or attempt to convert the stored string into JsonArray
		 * return
		 * 	JsonArray* pointer to a JsonArray which is held by this JsonPrimitive
		 * error:
		 * 	std::runtime_error when *str and *jsonArr are null
		 */
		JsonArray* getAsJsonArray();
};

class JsonObject{
	friend class JsonPrimitive;
	private:
		std::map<std::string, JsonPrimitive*> m; // the map that hold key - value pair
		/**
		 * parse a given string into JsonObject without validation.
		 * this can only be called by JsonPrimitive, or itself
		 * intput
		 * 	std::string the string to parse JsonObject
		 */
		JsonObject(std::string);
	public:
		/**
		 * default constructor to create an empty JsonObject
		 */
		JsonObject();
		/**
		 * get a JsonPrimitive based on the given string
		 * input
		 * 	std::string - name of the key
		 * output
		 * 	JsonPrimitive - the primitive data
		 * error
		 * 	std::runtime_error when no value associated with the key
		 */
		JsonPrimitive* get(std::string);
		/**
		 * put a string into this JsonObject
		 * input
		 * 	std::string the key of the object
		 * 	std::string* the pointer to the std::string associated with the key
		 */
		void put(std::string, std::string*);
		/** 
		 * put a JsonArray into this JsonObject
		 * input
		 * 	std::string the key of the object
		 * 	JsonArray* the pointer to the JsonArray associated with the key
		 */
		void put(std::string, JsonArray*);
		/**
		 * put a JsonObject into this JsonObject
		 * input
		 * 	std::string the key of the object
		 * 	JsonObject* the poitner to the JsonObject associated with the key
		 */
		void put(std::string, JsonObject*);
		/**
		 * parse a string into JsonObject with check
		 * input
		 * 	std::string* the pointer to the string to parse
		 */
		static JsonObject* parse(std::string*);
};

class JsonArray{
	friend class JsonPrimitive;
	private:
		std::vector<JsonPrimitive*> vec;
		/**
		 * prase the given string into a json array
		 * this will presume that the given string is in the correct form and parse it without verification
		 * input
		 * 	std::string - the string to parse to json array
		 */
		JsonArray(std::string);
	public:
		JsonArray();
		/**
		 * retreat a Json Primitive based on the index
		 * input:
		 * 	int - index number
		 * output:
		 * 	JsonPrimitive - the value associated with the index
		 * error:
		 * 	throw std::invalid_argument when the index is out of range
		 */
		JsonPrimitive* get(int);
		// store a string
		void put(std::string);
		// store a JsonArray
		void put(JsonArray);
		// store a JsonObject
		void put(JsonObject);
		/**
		 * remove and return a Json Primitive based on the index
		 * input:
		 * 	int - index number
		 * output:
		 *	JsonPrimitive - the removed object associated with the index
		 * error:
		 * 	when the index if out of range
		 */
		JsonPrimitive* remove(int);
		/**
		 * pase a srting to JsonArray with validation.
		 * input
		 * 	std::string a string to prase to JsonArray
		 * return
		 * 	an instane of this class
		 * error
		 * 	when the string is invalid
		 */
		static JsonArray* parse(std::string);
};
