#include <cstddef>
#include <ios>
#include <iostream>
#include <string>
#include <iomanip>
template<class T> class Matrix {
	private:
		unsigned int num_x, num_y;
		T* arr;
	public:
		/**
		 * @brief:
		 * default constructor
		 */
		Matrix();
		/**
		 * @brief:
		 * create a matrix with specified size
		 * @params:
		 * unsigned int: x dimention (number of columns)
		 * unsigned int: y dimention (number of rows)
		 */
		Matrix(unsigned int, unsigned int);

		/**
		 * @brief:
		 * create a matrix with the size and copy down the given array
		 * @params:
		 * unsigned int: x dimention (number of columns)
		 * unsigned int: y dimention (number of rows)
		 * const T*: ptr to the array to copy
		 */
		Matrix(unsigned int, unsigned int, const T*);

		/**
		 * @brief
		 * get an element of the matrix at the given position
		 * @params:
		 * size_t: x location
		 * size_t: y location
		 * @return
		 * T: element of the given position
		 * @err
		 * when the index is out of bound
		 */
		T get(size_t, size_t);

		/**
		 * @brief
		 * set an given a position, set that position to a given element
		 * @params
		 * size_t: the x index
		 * size_t: the y index
		 * @err
		 * when the index is out of bound
		 */
		void set(size_t, size_t, T);

		/**
		 * @brief:
		 * sum 2 matricies
		 * @params:
		 * const Matrix<U>& the other maitrx 
		 * 
		 */
		template<class U> Matrix<U> operator+ (const Matrix<U>&);
		template<class U> friend std::ostream& operator<< (std::ostream& os, const Matrix<U>&);

};

template<class T> Matrix<T>::Matrix(){
	std::cout << "default" << std::endl;
}

template<class T> Matrix<T>::Matrix(unsigned int x, unsigned int y){
	this->num_x = x;
	this->num_y = y; 
	this->arr = new T[x*y];
}

template<class T> Matrix<T>::Matrix(unsigned int x, unsigned int y, const T* arr):Matrix(x,y){
	for (int i = 0; i < x*y; i++) this->arr[i] = arr[i];
}

template<class T> T Matrix<T>::get(size_t x, size_t y){
	if (x >= num_x || y > num_y) throw "the given index exceed the matrix";
	return *arr[x+y*num_x];
}

template<class T> void Matrix<T>::set(size_t x, size_t y, T ele){
	if (x >= num_x || y > num_y) throw "the given index exceed the matrix";
	*arr[x+y*num_x] = ele;
}

template<class T>
template<class U>
Matrix<U> Matrix<T>::operator+ (const Matrix<U>& other){
	if (this->num_x != other.num_x || this->num_y != other.num_y) throw "2 matricies are not of the same sizes";
	T arr[this->num_x * this->num_y];
	for (int i=0;i<this->num_x*this->num_y;i++) arr[i] = this->arr[i] + other.arr[i];
	return *(new Matrix<T>(this->num_x,this->num_y,arr));
}

template<class U>
std::ostream& operator<< (std::ostream& os, const Matrix<U>& other) {
	os << std::fixed << std::setprecision(4);
	for (int y=0; y<other.num_y; y++){
		for (int x=0; x<other.num_x; x++){
			os << std::setw(8) << other.arr[x+y*other.num_x] << " ";
		}
		os << "\n" ;
	}
	return os;
}