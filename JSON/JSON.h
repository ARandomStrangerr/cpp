#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <regex>

enum class Type;
class Primitive;
class Object;
class Array;

enum class Type{STRING, NUMBER, BOOLEAN, ARRAY, OBJECT};

/**
this class must be deleted after use since it allocates memory dynamically.
the it design in a way such that
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
		Primitive get(std::string);
		void remove(std::string);
		void set(std::string, std::string);
		void set(std::string, double);
		void set(std::string, bool);
		void set(std::string, Object);
		void set(std::string, Array);
		friend std::ostream& operator<< (std::ostream&, const Object&);
		static Object parse(const std::string&);
};

class Array{
	private:
		std::vector<Primitive*> vec;
	public:
		Primitive* getPtr(int);
		Primitive* deletePtr(int);
		void setPtr(int, Primitive*);
};
