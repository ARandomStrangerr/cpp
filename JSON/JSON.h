#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <regex>

class Parse;
class Primitive;
class Object;
class Array;

enum Type {STRING, NUMBER, BOOLEAN, ARRAY, OBJECT};

class Parse {
	public:
		/**
		@brief:
		get the key of the key:value pair.
		parse the string until it see ':'
		clean up the string by delete ' ' and quote at 2 ends
		check if the string is fit to be variable by regex
		@params:
		int& pass by reference of the begin index to start parsing
		std::string the string to parse
		@return:
		std::string a non-dynamicall-allocated string represent key.
		@error:
		std::runtime_error when it is the end of the given string and it is still not meet ':'.
		std::runtime_error when the variable is not fit to be variable name
		*/
		static std::string getKey(int&, const std::string&);

		static std::string getString(int&, const std::string&);

		static double getNumber(int&, const std::string&);

		static bool getBoolean(int&, const std::string&);

		/**
		@breif:
		get the Object represent value of key:value pair
		parse until pair '{' and '}' are equal
		@params:
		int& pass by reference of the begin index to start parsing
		std::string the string to parse
		@return:
		Object dynaically-allocated object
		@error:
		std::runtime_error when it is the end of the given string and pair '{' and '}' are not equal
		std::runtime_error when there is a character out of place
		*/
		static Object getObject(int&, const std::string&);
		static Array getArray(int&, const std::string&);
	friend class Primitive;
	friend class Object;
	friend class Array;
};

/**
this class must be deleted after use since it allocates memory dynamically.
it design in a way such that when the original given variable is deleted, the varibale in this class does not point to a nulptr or deleted pointer.
*/
class Primitive{
	private:
		std::string str;
		double num;
		bool boo;
		Object* objPtr;
		Array* arrPtr;
		Type type;
	public:
		/**
		@brief
		constructor for a string.
		this class dynamically allocate memory a copy of the given string
		*/
		Primitive(std::string);

		/**
		@brief
		constructor for a string pointer.
		this class hold a copy of the deference of this string
	 	*/
		Primitive(std::string*);

		/**
		@brief
		constructor for a number.
		directly set this class number to the given number because the value of number of this class retains when the given number is expired
	 	*/
		Primitive(double);

		/**
		@brief
	 	constructor poitner for a number.
		this class hold copy value of the given pointer because the value of number of this class is dangling when the given poitner is deleted
		*/
		Primitive(double*);

		/**
		@brief
		constructor for a boolean.
		simlar to double constructor
	 	*/
		Primitive(bool);

		/**
		@brief
		constructor for a boolean pointer.
		simlar to double pointer constructor
	 	*/
		Primitive(bool*);

		/**
		@brief
		constructor for an object.
		address-of given object then set it to this object poitner.
		the given object must be dinamically allocated because when it is release, this object poitner points to nothing.
	 	*/
		Primitive(Object);

		/**
		@brief
		constructor for an object.
		this poitner points to the given object poitner.
		the given object must be dinamically allocated because when it is release, this object poitner points to nothing.
	 	*/
		Primitive(Object*);

		/**
		@brief
		constructor for an array.
		similar to object constructor
	 	*/
		Primitive(Array);

		/**
		@brief
		constructor for an array.
		similar to object poitner constructor
	 	*/
		Primitive(Array*);

		/**
		@brief
	 	destructor.
		call the destructor of the data that this hold
		*/
		~Primitive();

		/**
	 	@breif:
		get this primitive as String
		@return:
		std::string a string represents this primitive
		@error:
		when this primitive does not hold a string
		*/
		std::string getStr();

		/**
	 	@brief:
		get this primitive as Number
		@return:
		double a number represent this primitive
		@error:
		when this primitive does not hold a number
		*/
		double getNum();

		/**
	 	@brief:
		get this primitive as Boolean
		@return:
		Boolean a number represent this primitive
		@error:
		when this primitive does not hold a Boolean
		*/
		bool getBool();

		/**
	 	@brief:
		get this primitive as Object
		@return:
		Object a number represent this primitive
		@error:
		when this primitive does not hold an Object
		*/
		Object getObj();

		/**
	 	@brief:
		get this primitive as Array
		@return:
		Array a number represent this primitive
		@error:
		when this primitive does not hold an Array
		*/
		Array getArr();

		friend std::ostream& operator<< (std::ostream&, const Primitive&);
		friend class Parse;
};

class Object{
	private:
		std::unordered_map<std::string, Primitive*> map;
	public:
		/**
		@brief
		defalut constructor.
		just create a map here
		*/
		Object();
		//~Object();
		/**
		@brief
		go through each element in the map, release the memory.
		empty the map.
		then set the poiner of the map to nulltpr
		*/
		Primitive get(std::string);

		/**
		@brief
		remove an element out of the map then de-allocate the element
		@param
		std::string name of the key
		*/
		void remove(std::string);

		/**
		@brief
		add a string element into the map
		if the key is already allocated with an element, the allocated element will be de-allocate
		@params
		std::string key
		std::string value
		*/
		void set(std::string, std::string);

		/**
		@brief
		add a number element into the map
		if the key is already allocated with an element, the allocated element will be de-allocate
		@params
		std::string key
		double value
		*/
		void set(std::string, double);

		/**
		@brief
		add a boolean element into the map
		if the key is already allocated with an element, the allocated element will be de-allocate
		@params
		std::string key
		bool value
		*/
		void set(std::string, bool);

		/**
		@brief
		add an object element into the map
		if the key is already allocated with an element, the allocated element will be de-allocate
		@params
		std::string key
		Object value
		*/
		void set(std::string, Object);

		/**
		@brief
		add an Array element into the map
		if the key is already allocated with an element, the allocated element will be de-allocate
		@params
		std::string key
		Array value
		*/
		void set(std::string, Array);

		/**
		@brief
		parse a string to an object
		@param
		const std::string& the string to parse
		@error
		std::runtime_error when a character is out of place, for more information, read Parse class and each of its function.
		*/
		static Object parse(const std::string&);
		friend std::ostream& operator<< (std::ostream&, const Object&);
};

class Array{
	private:
		std::vector<Primitive*>* vec;
	public:
		Array();
		Primitive get(int);
		void insert(std::string);
		void insert(double);
		void insert(bool);
		void insert(Object);
		void insert(Array);
		void remove(int);
		void setPtr(int, Primitive*);
		Array parse(const std::string&);
};
