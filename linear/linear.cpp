#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>

template <class T> class Matrix {
private:
unsigned int num_x, num_y;
  T *arr;

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
  Matrix(unsigned int, unsigned int, const T *);

  /**
   * @brief:
   * clone a given matrix
   * @params:
   * Matrix<T> the matrix to clone
   */
  Matrix(Matrix<T> *);

  /**
   * @brief:
   * destructor
   */
  ~Matrix<T>();

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
   * @breif
   * return a new matrix which is a transpose of this matrix
   */
  Matrix<T> transpose();

  /**
   * @brief
   * reduce the matrix to the row echelon form
   * @return
   * a new instance of this matrix which is in the row echelon form
   */
  Matrix<T> gaussElimination();

  /**
   * @brief
   * solve the matrix using LU decomposition. requirement is that this matrix
   * now can solve matrix when diagonal = 0
   * partial pivoting is not implemented yet
   * @params
   * Matrix<T> the right hand side of the Ax=b
   * @return
   * a column vector as the solution of the system Ax=b
   * @error
   * the current matrix is not square.
   * the right hand side is in correct length.
   */
  Matrix<T> solve(Matrix<T>);

  Matrix<double> inverse();

  /**
   * @brief:
   * sum 2 matricies
   * @params:
   * const Matrix<U>& the other matrix
   */
  Matrix<T> operator+(const Matrix<T> &);

  /**
   * @breif
   * subtract 2 matricies
   * @params:
   * const Matrix<U>& the other matrix
   */
  Matrix<T> operator-(const Matrix<T> &);

  /**
   * @brief
   * matrix multiplication. should implementation other algorithm
   * due to substract cancellation when addition with small number.
   * recommend to change it to Cannon's, Strassen's.
   * for the sake of simplicity, the standadrd matrix multiplication is
   implemented
    @params
   * const Matrix<U>& the other matrix
   * @err
   * when the size of the matrix are incorrect
   */
  Matrix<T> operator*(const Matrix<T> &);

  /**
   * @breif
   * scalar multiplication
   * @params
   * T scalar number to multiply with
   * @return
   * return a new matrix which is the scalar multiplication of this matrix
   */
  Matrix<T> operator*(T);

  /**
   * @brief
   * check if 2 matricies are equal to eachother
   * @return
   * true - if 2 matricies are equal to eachother
   * false - if 2 matricies are not equal to eachother
   */
  bool operator==(const Matrix<T>);

  template <class U>
  friend std::ostream &operator<<(std::ostream &os, const Matrix<U> &);
};

template <class T> Matrix<T>::Matrix() { std::cout << "default" << std::endl; }

template <class T> Matrix<T>::Matrix(unsigned int x, unsigned int y) {
  this->num_x = x;
  this->num_y = y;
  this->arr = new T[x * y];
}

template <class T>
Matrix<T>::Matrix(unsigned int x, unsigned int y, const T *arr) : Matrix(x, y) {
	for (int i = 0; i < x * y; i++) this->arr[i] = arr[i];
}

template <class T>
Matrix<T>::Matrix(Matrix<T> *matrixPtr) : Matrix(matrixPtr->num_x, matrixPtr->num_y, matrixPtr->arr) {}

template <class T> Matrix<T>::~Matrix<T>() {
  delete[] this->arr;
  this->arr = nullptr;
}

template <class T> T Matrix<T>::get(size_t x, size_t y) {
  if (x >= num_x || y > num_y) throw "the given index exceed the matrix";
  return arr[x + y * num_x];
}

template <class T> void Matrix<T>::set(size_t x, size_t y, T ele) {
  if (x >= num_x || y > num_y) throw "the given index exceed the matrix";
  arr[x + y * num_x] = ele;
}

template <class T> Matrix<T> Matrix<T>::transpose() {
  T arr[this->num_x * this->num_y];
  for (int x = 0; x < this->num_x; x++) {
    for (int y = 0; y < this->num_y; y++) arr[y + x * this->num_y] = this->arr[x + y * this->num_x];
  }
  return *(new Matrix<T>(this->num_y, this->num_x, arr));
}

template <class T> Matrix<T> Matrix<T>::gaussElimination() {
  /*
   * the diagonal is (x,y) when x=y so one number will justify
   * the diagonal run until y-1 since the last row does not have anything below
   * it to eliminate proof that loop will eliminate: num_y is an infinite number
   * which does not increase for each iteration by increasing the counter by 1,
   * eventually the variable diagonal will reach num_y
   */
  Matrix<T> *returnMatrix = new Matrix<T>(num_x, num_y, arr);
  for (int diagonal = 0; diagonal < num_y - 1; diagonal++) {
    // if the current row has 0 diagonal swap it with the bottom row
    if (returnMatrix->arr[diagonal + diagonal * num_x] == 0.0) {
      for (int x = diagonal; x < num_x; x++) {
        T temp = returnMatrix->arr[x + diagonal * num_x];
        returnMatrix->arr[x + diagonal * num_x] = returnMatrix->arr[x + (num_y - 1) * num_x];
        returnMatrix->arr[x + (num_y - 1) * num_x] = temp;
      }
    }
    // the eliminate rows are row beneath the mail diagonal row; hence,
    // eliminate row is diagonal + 1 till the last row
    for (int eliminateRow = diagonal + 1; eliminateRow < num_y;
         eliminateRow++) {
      double m = returnMatrix->arr[diagonal + eliminateRow * num_x] / returnMatrix->arr[diagonal + diagonal * num_x];
      returnMatrix->arr[diagonal + eliminateRow * num_x] = 0;
      for (int x = diagonal + 1; x < num_x; x++) {
        returnMatrix->arr[x + eliminateRow * num_x] -= m * returnMatrix->arr[x + diagonal * num_x];
      }
    }
  }
  return *(returnMatrix);
}

template <class T> Matrix<T> Matrix<T>::solve(Matrix<T> rhs) {
  // check for error
  if (num_x != num_y) throw "this matrix is not square matrix";
  if (rhs.num_y != num_y) throw "the right hand side column vector does not match with this matrix";
  // copy the rhs
  Matrix<T> *rhsCopy = new Matrix<T>(1,rhs.num_y);
  // make the pivor array
  int pvt[num_y];
  for (int i = 0; i < num_y; i++) pvt[i] = i;
  // step 1: PA = PLU
  Matrix<T> *returnMatrix = new Matrix<T>(this);
  for (int diagonal = 0; diagonal < num_y - 1; diagonal++) {
    // the current diagonal has 0 pivot entry, swap it with the botton row
    if (returnMatrix->arr[diagonal + diagonal * num_x] == 0) {
      for (int x = 0; x < num_x; x++) {
        T temp = returnMatrix->arr[x + diagonal * num_x];
        returnMatrix->arr[x + diagonal * num_x] = returnMatrix->arr[x + (num_y - 1) * num_x];
        returnMatrix->arr[x + (num_y - 1) * num_x] = temp;
      }
      // swap the pivot entries
      // this can happens due to the swap happened only once at the current row; hence, pvt[pivot_row] = pivot_row always, not some other number
      pvt[diagonal] = pvt[num_y - 1];
      pvt[num_y - 1] = diagonal;
    }
    for (int eliminateRow = diagonal + 1; eliminateRow < num_y; eliminateRow++) {
      double m = returnMatrix->arr[diagonal + eliminateRow * num_x] / returnMatrix->arr[diagonal + diagonal * num_x];
      returnMatrix->arr[diagonal + eliminateRow * num_x] = m;
      for (int x = diagonal + 1; x < num_x; x++) returnMatrix->arr[x + eliminateRow * num_x] -= m * returnMatrix->arr[x + diagonal * num_x];
    }
  }
  // step 2: perform swap row of the rhs
  for (int y = 0; y < num_y; y++) rhsCopy->arr[y] = rhs.arr[pvt[y]];
  // step 3: now we have PLUx=Pb, let z=Ux, solve the system (PL)z=Pb
  for (int y = 1; y < num_y; y++) {
    for (int x = 0; x < y; x++) {
      rhsCopy->arr[y] -= rhsCopy->arr[x] * returnMatrix->arr[x + y * num_x];
    }
  }
  // step 4: solve the system Ux=z
  for (int y = num_y - 1; y >= 0; y--) {
    for (int x = num_x - 1; x > y; x--) {
      rhsCopy->arr[y] -= rhsCopy->arr[x] * returnMatrix->arr[x + y * num_x];
    }
    rhsCopy->arr[y] = rhsCopy->arr[y] / returnMatrix->arr[y + y * num_x];
  }
  delete returnMatrix;
  return rhsCopy;
}

template <class T> Matrix<double> Matrix<T>::inverse(){
  // copy this matrix to decompose
  Matrix<T> *copyMatrix = new Matrix<T>(this);
  // pivor array
  int pvt[num_y];
  for (int i = 0; i < num_y; i++) pvt[i] = i;
  // decompose the matrix to PLU
  for (int diagonal = 0; diagonal < num_y - 1; diagonal++) {
    // the current diagonal has 0 pivot entry, swap it with the botton row
    if (copyMatrix->arr[diagonal + diagonal * num_x] == 0) {
      for (int x = 0; x < num_x; x++) {
        T temp = copyMatrix->arr[x + diagonal * num_x];
        copyMatrix->arr[x + diagonal * num_x] = copyMatrix->arr[x + (num_y - 1) * num_x];
        copyMatrix->arr[x + (num_y - 1) * num_x] = temp;
      }
      // swap the pivot entries
      pvt[diagonal] = pvt[num_y - 1];
      pvt[num_y - 1] = diagonal;
    }
    for (int eliminateRow = diagonal + 1; eliminateRow < num_y; eliminateRow++) {
      double m = copyMatrix->arr[diagonal + eliminateRow * num_x] / copyMatrix->arr[diagonal + diagonal * num_x];
      copyMatrix->arr[diagonal + eliminateRow * num_x] = m;
      for (int x = diagonal + 1; x < num_x; x++) copyMatrix->arr[x + eliminateRow * num_x] -= m * copyMatrix->arr[x + diagonal * num_x];
    }
  }
  // the inverse matrix
  Matrix<double>* returnMatrix = new Matrix<double>(num_x, num_y);
  for (int y=0;y<num_y;y++){
    returnMatrix->arr[pvt[y]+y*num_x]=1;
  }
  // solve for each column of the return matrix
  for (int col=0; col < num_x; col++){
    // solve the forward sub
    for (int y = 1; y < num_y; y++){
      for (int x = 0; x < y; x++){
        returnMatrix->arr[col+y*num_x]-=returnMatrix->arr[col+x*num_x]*copyMatrix->arr[x+y*num_x];
      }
    }
    // solve the backward sub
    for (int y = num_y-1; y >= 0; y--){
      for (int x = num_x-1; x > y; x--){
        returnMatrix->arr[col+y*num_x] -= returnMatrix->arr[col+x*num_x] * copyMatrix->arr[x+y*num_x];
      }
      returnMatrix->arr[col+y*num_x] = returnMatrix->arr[col+y*num_x] / copyMatrix->arr[y+y*num_x];
    }
  }
  std::cout<<returnMatrix->num_x<<std::endl;
  //delete copyMatrix;
  return returnMatrix;
}

template <class T> Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) {
  if (this->num_x != other.num_x || this->num_y != other.num_y)
    throw "2 matricies are not of the same sizes";
  T arr[this->num_x * this->num_y];
  for (int i = 0; i < this->num_x * this->num_y; i++)
    arr[i] = this->arr[i] + other.arr[i];
  Matrix<T> matrix(this->num_x, this->num_y, arr);
  std::cout << &matrix << std::endl;
  return matrix;
}

template <class T> Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) {
  if (this->num_x != other.num_x || this->num_y != other.num_y)
    throw "2 matricies are not of the same size";
  T arr[this->num_x * this->num_y];
  for (int i = 0; i < this->num_x * this->num_y; i++)
    arr[i] = this->arr[i] - other.arr[i];
  return *(new Matrix<T>(this->num_x, this->num_y, arr));
}

template <class T> Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) {
  if (this->num_x != other.num_y) throw "The column of first column is not equal to the row of the second matrix";
  T arr[this->num_y * other.num_x];
  for (int y = 0; y < other.num_y; y++) {
    for (int x = 0; x < this->num_x; x++) {
      arr[x + other.num_x * y] = 0;
      for (int i = 0; i < this->num_x; i++) {
        arr[x + other.num_x * y] += this->arr[i + y * this->num_x] * other.arr[x + i * other.num_y];
      }
    }
  }
  return *(new Matrix<T>(other.num_x, this->num_y, arr));
}

template <class T> Matrix<T> Matrix<T>::operator*(T scalar) {
  T arr[this->num_x * this->num_y];
  for (int i = 0; i < this->num_x * this->num_y; i++)
    arr[i] = this->arr[i] * scalar;
  return *(new Matrix<T>(this->num_x, this->num_y, arr));
}

template <class T> bool Matrix<T>::operator==(const Matrix<T> other) {
  if (this->num_x != other.num_x || this->num_y != other.num_y)
    return false;
  for (int i = 0; i < this->num_x * this->num_y; i++)
    if (this->arr[i] != other.arr[i])
      return false;
  return true;
}

template <class U>
std::ostream &operator<<(std::ostream &os, const Matrix<U> &other) {
  os << std::fixed << std::setprecision(4);
  for (int y = 0; y < other.num_y; y++) {
    for (int x = 0; x < other.num_x; x++) {
      os << std::setw(8) << other.arr[x + y * other.num_x] << " ";
    }
    os << "\n";
  }
  return os;
}
