#include <iostream>
#include <map>
#include <vector>

class JsonPrimitive;
class JsonObject;
class JsonArray;

class JsonPrimitive{
	friend std::ostream& operator<<(std::ostream&,JsonPrimitive&);
	friend class JsonObject;
	friend class JsonArray;
	private:
		std::string* strPtr;
		JsonObject* jsonObjPtr;
		JsonArray* jsonArrPtr;

		std::string beautifyDisplay(int);
	public:
		/**
		 * get the string holding by this JsonPrimitive.
		 * return:
		 * 	std::string* a pointer to the string.
		 */
		std::string* getStr();
		/**
		 * convert this json primitive to json object
		 * return:
		 *  JsonObject* a converstion of this class to json object
		 */
		JsonObject* getJsonObj();
		/**
		 * convert this json primitive to json array
		 * return:
		 *  JsonArray* a conversion of this pointer to json array
		 */
		JsonArray* getJsonArr();
};

class JsonObject{
	friend std::ostream& operator<<(std::ostream&,JsonObject&);
	friend class JsonPrimitive;
	private:
		std::map<std::string, JsonPrimitive*>* mapPtr;
		/**
		 * get the string representative of this JsonObject
		 * input
		 * 	int - number of tab (\t)
		 */
		std::string beautifyDisplay(int);
	public:
		/**
		 * default constructor
		 */
		JsonObject();
		/**
		 * add a string to this json object
		 * input:
		 * 	std::string - key
		 * 	std::string* - pointer to the store object. MUST BE DYNAMICALLY ALLOCATED
		 */
		void add(std::string, std::string*);
		/**
		 * add a JsonObject to this json object
		 * input
		 * 	std::string - key
		 * 	JsonObject* - pointer to a json object. MUST BE DYNAMICALLY ALLOCATED
		 */
		void add(std::string, JsonObject*);
		/**
		 * add a JsonArray to this json object
		 * input
		 * 	std::string - key
		 *	JsonArray* - pointer to a json array. MUST BE DYNAMICALLY ALLOCATED
		 */
		void add(std::string, JsonArray*);
		/**
		 * get an element associated with the key
		 * input
		 * 	std::string - name of the key
		 * return
		 * 	JsonPrimitive* pointer to the object
		 * error
		 * 	std::string - error message. when the given key does not associate with any value
		 */
		JsonPrimitive* get(std::string);
		/**
		 * remove and return an element associated with the key
		 * input
		 * 	std::string - key
		 * return
		 * 	JsonPrimitive* - pointer to. the removed object
		 * error
		 * 	std::string - error message. when the given key does not associate with any value
		 */
		JsonPrimitive* remove(std::string);
};

class JsonArray{
	friend std::ostream& operator<<(std::ostream&, JsonArray&);
	friend class JsonObject;
	friend class JsonPrimitive;
	private:
		std::vector<JsonPrimitive*>* vec;
		std::string beautifyDisplay(int);
	public:
		/**
		 * default constructor
		 */
		JsonArray();
		/**
		 * add a string into this json array
		 * input:
		 * 	std::string* - pointer to a string. MUST BE DYNAMICALLY ALLOCATED.
		 */
		void add(std::string*);
		/**
		 * add a json object into this json array
		 * input:
		 * 	JsonObject* - pointer to a JsonObject. MUST BE DYNAMICALLY ALLOCATED.
		 */
		void add(JsonObject*);
		/**
		 * add a json array into this json array
		 * input:
		 * 	JsonArray* - pointer to a JsonArray. MUST BE DYNAMICALLY ALLOCATED.
		 */
		void add(JsonArray*);
		/**
		 * retreat an element based on index from this json array
		 * input:
		 * 	int - index
		 * return:
		 * 	JsonPrimitive* - pointer to the element associated with the index
		 * error:
		 * 	std::string - error message. when the index is out of bound
		 */
		JsonPrimitive* get(int);
		/**
		 * remove and return an element based on index from this json array
		 * input:
		 * 	int - index
		 * return:
		 * 	JsonPrimitive* - pointer to the removed element.
		 * error: 
		 *	std::string - error message. when the index is out of bound
		 */
		JsonPrimitive* remove(int);
};
