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
		JsonPrimitive(std::string);
		JsonPrimitive(JsonObject);
		JsonPrimitive(JsonArray);
	public:
		// return the stored JsonObject or convert the stored string to JsonObject
		JsonObject getAsJsonObject();
		// return the stored JsonArray or convert the sotred string to JsonArray
		JsonArray getAsJsonArray();
};

class JsonObject{
	private:
		std::map<std::string, JsonPrimitive*> m;
	public:
		// parse a given string into JsonObject
		JsonObject(std::string);
		// get a JsonPrimitive based on the given key
		JsonPrimitive get(std::string);
		// put a string
		void put(std::string, std::string);
		// put a JsonArray
		void put(std::string, JsonArray);
		// put a JsonObject
		void put(std::string, JsonObject);
};

class JsonArray{};
