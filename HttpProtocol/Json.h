#include <ostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <regex>

class JsonPrimitive;
class JsonObject;
class JsonArray;

enum PrimitiveType {
	STRING, NUMBER, BOOLEAN, JSON_ARRAY, JSON_OBJECT
};
/**
 * variable boolVal, numVal are fundamental types, when JsonPrimitive is out of scope, the memory will be claim automatically.
 * variable strVal manage its own memory, same case as above.
 * variable jsonObjPtr, jsonArrPtr inteded to change the original object, therefore when delete, those objects must be deleted manually.
 */
class JsonPrimitive {
	friend class JsonObject; // friend to JsonObject class so it has access to the private memeber of the class
	friend class JsonArray; // friend to JsonArray class so it has access to the private memeber of the class	private:
	private:
		PrimitiveType type;
		std::string strVal; // string object, when use the assing operation will create a copy of the given string
		bool boolVal; // fundamental data type, when use the assing operation will create a copy of the given boolean value
		double numVal; // fundamental data type, when use the assing operation will create copy of the given double value
		JsonObject* jsonObjPtr; // forward declaration, must be ptr type, highly recommended to be dynamically allocated
		JsonArray* jsonArrPtr; // forward declaration, must be ptr type, highly recommended to be dynamically allocated
		/**
		 * @brief: format display the data hold by this instance
		 */
		std::string beautifyDisplay(int) const;
	public:
		JsonPrimitive();
		//~JsonPrimitive();
		/**
		 * @brief: get this JsonPrimitive as string
		 * @return: a copy of string represent this JsonPrimitive
		 * @throw: when this object does not represent as string
		 */
		std::string getAsString();
		/**
		 * @brief: get this JsonPrimitive as boolean
		 * @return: a copy of boolean represent this JsonPrimitive
		 * @throw: when this object does not represent as boolean
		 */
		bool getAsBoolean();
		/**
		 * @brief: get this JsonPrimitive as number
		 * @return: a copy of double represent this JsonPrimitive
		 * @throw: when this object does not represent as number
		 */
		double getAsNumber();
		/**
		 * @brief: get this JsonPrimitive as JsonObject
		 * @return: a reference of JsonObject ptr represent this JsonPrimitive
		 * @throw: when this object does not represent as JsonObject
		 */
		JsonObject& getAsObj();
		/**
		 * @brief: get this JsonPrimitive as JsonArray
		 * @return: a reference of JsonArray represent this JsonPrimitive
		 * @return: when this object does not represent as JsonArray
		 */
		JsonArray& getAsArr();
};

class JsonObject {
	friend class JsonPrimitive;
	friend std::ostream& operator<<(std::ostream&, const JsonObject&);
	private:
		std::map<std::string, JsonPrimitive> map; // the map to hold collection of key-value pair	
		/**
		 * @brief: format display hold by this instance
		 */
		std::string beautifyDisplay(int tab) const;
	public:
		static JsonObject& parse(std::string);
		//~JsonObject();
		/**
		 * @brief: return the number of key-value pairs in this collection
		 */
		int size();
		/**
		 * @brief: get a pointer of JsonPrimitive assiciated with the given key.
		 * @params:
		 * 	std::string - key
		 * @return:
		 * 	a pointer associated with the key
		 * @throw:
		 * 	std::out_of_range - when the key does not exist
		 */
		JsonPrimitive& get(std::string);
		/**
		 * @brief: remove and return a pointer of JsonPrimitive associated with the given key.
		 * @params:
		 * 	std::string - key
		 * @return:
		 * 	a pointer removed from the map
		 * @throw:
		 * 	std::out_of_range - when the key does not exist
		 */
		JsonPrimitive& remove(std::string);
		/**
		 * @brief: add a key-string pair into this collection.
		 * @params:
		 * 	std::string - key
		 * 	std::string - value
		 */
		void add(std::string, std::string);	
		/**
		 * @brief: add a key-boolean pair into this collection.
		 * @param:
		 * 	std::string - key
		 * 	bool - value
		 */
		void add(std::string, bool);
		/**
		 * @brief: add a key-number pair into this collection
		 * @param:
		 * 	std::string - key
		 * 	double - value
		 */
		void add(std::string, double);
		/**
		 * @brief: add a key-JsonArray pair into this collection, the original object highly recommended to be dynamically allocated
		 * @param:
		 * 	std::string - key
		 * 	JsonArray* - ptr to the value
		 */
		void add(std::string, JsonArray*);
		/**
		 * @brief: add a key-JsonObject pair into this collection, the original object highly recommended to be dynamically allocated
		 * @param:
		 * 	std::string - key
		 * 	JsonObject*  ptr to the value
		 */
		void add(std::string, JsonObject*);
};

class JsonArray {
	friend class JsonPrimitive;
	friend std::ostream& operator<< (std::ostream&, const JsonArray&);
	private:
		std::vector<JsonPrimitive> vec;
		/**
		 * @brief: format display holds by this instance
		 */
		std::string beautifyDisplay(int tab) const;
	public:
		static JsonArray& parse (std::string);
		//~JsonArray();
		/**
		 * @brief: return the number of item in this collection
		 */
		int length();
		/**
		 * @brief: get an item at a specific position
		 * @params:
		 * 	int - index position
		 * @throw: when the position is out of bound
		 */
		JsonPrimitive& get(int);
		/**
		 * @brief: remove and return a pointer of JsonPrimitive at the given index
		 * @param: 
		 * 	int - index position
		 * @throw: when the position is out of bound
		 */
		JsonPrimitive& remove(int);
		/**
		 * @brief: add a number into the json array
		 * @param:
		 * 	double - value
		 */
		void add(double);
		/**
		 * @brief: add a string into the array
		 * @param:
		 * 	std::string - value
		 */
		void add(std::string);
		/**
		 * @brief: add a boolean into the array
		 * @param:
		 * 	bool - value
		 */
		void add(bool);
		/**
		 * @brief: add a json object ptr into the array
		 * @param:
		 * 	JsonObject* - the ptr to add
		 */
		void add(JsonObject*);
		/**
		 * @brief: add a json array ptr into the array
		 * @param:
		 * 	JsonArray* - the ptr to add
		 */
		void add(JsonArray*);
};
