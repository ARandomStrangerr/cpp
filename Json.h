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
		std::string* str;
		JsonObject* jsonObj;
		JsonArray* jsonArr;
		JsonPrimitive(std::string); // store the arg as string
		JsonPrimitive(JsonObject); // store the arg as JsonObject
		JsonPrimitive(JsonArray); // store the arg as JsonArray
	public:
		/**
		 * return the stored string stored in this object
		 * Will throw error if there is the 'str' pointer is null
		 */
		std::string getAsString();
		// return the stored JsonObject or attempt to convert the stored string to JsonObject
		JsonObject getAsJsonObject();
		// return the stored JsonArray or attempt to convert the sotred string to JsonArray
		JsonArray getAsJsonArray();
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
		 * get a JsonPrimitive based on the given string
		 * input
		 * 	std::string - name of the key
		 * output
		 * 	JsonPrimitive - the primitive data
		 * error
		 * 	when not value associated with the key
		 */
		JsonPrimitive get(std::string);	// get a JsonPrimitive based on the given key
		void put(std::string, std::string); // put a string	
		void put(std::string, JsonArray); // put a JsonArray
		void put(std::string, JsonObject); // put a JsonObject
		static JsonObject parse(std::string); // parse a string to JsonObject with validation. return an instance of this class
};

class JsonArray{
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
		/**
		 * retreat a Json Primitive based on the index
		 * input:
		 * 	int - index number
		 * output:
		 * 	JsonPrimitive - the value associated with the index
		 * error:
		 * 	throw an error when the index is out of range
		 */
		JsonPrimitive get(int);
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
		void remove(int);
		/**
		 * pase a srting to JsonArray with validation.
		 * input
		 * 	std::string a string to prase to JsonArray
		 * return
		 * 	an instane of this class
		 * error
		 * 	when the string is invalid
		 */
		static JsonArray parse(std::string);
};
