#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <regex>

enum class Type;
class Parse;
class Primitive;
class Object;
class Array;

enum class Type{STRING, NUMBER, BOOLEAN, ARRAY, OBJECT};

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
		Type type;
		std::string str;
		double num;
		bool boo;
		Object* objPtr;
		Array* arrPtr;
	public:
		/**
		constructor for a string.
		this class dynamically allocate memory a copy of the given string
		*/
		Primitive(std::string);

		/**
		constructor for a string pointer.
		this class hold a copy of the deference of this string
	 	*/
		Primitive(std::string*);

		/**
		constructor for a number.
		directly set this class number to the given number because the value of number of this class retains when the given number is expired
	 	*/
		Primitive(double);

		/**
	 	constructor poitner for a number.
		this class hold copy value of the given pointer because the value of number of this class is dangling when the given poitner is deleted
		*/
		Primitive(double*);

		/**
		constructor for a boolean.
		simlar to double constructor
	 	*/
		Primitive(bool);

		/**
		constructor for a boolean pointer.
		simlar to double pointer constructor
	 	*/
		Primitive(bool*);

		/**
		constructor for an object.
		address-of given object then set it to this object poitner.
		the given object must be dinamically allocated because when it is release, this object poitner points to nothing.
	 	*/
		Primitive(Object);
		/**
		constructor for an object.
		this poitner points to the given object poitner.
		the given object must be dinamically allocated because when it is release, this object poitner points to nothing.
	 	*/
		Primitive(Object*);

		/**
		constructor for an array.
		similar to object constructor
	 	*/
		Primitive(Array);

		/**
		constructor for an array.
		similar to object poitner constructor
	 	*/
		Primitive(Array*);

		/**
	 	destructor
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
};

class Object{
	private:
		std::unordered_map<std::string, Primitive*> map;
	public:
		Object();
		Object(std::string);
		Primitive get(std::string);
		void remove(std::string);
		void set(std::string, std::string);
		void set(std::string, double);
		void set(std::string, bool);
		void set(std::string, Object);
		void set(std::string, Array);
		static Object parse(const std::string&);
		friend std::ostream& operator<< (std::ostream&, const Object&);
};

class Array{
	private:
		std::vector<Primitive*>* vec;
	public:
		Array();
		Array(std::string);
		Primitive* getPtr(int);
		Primitive* deletePtr(int);
		void setPtr(int, Primitive*);
};
