#include "linear.cpp"
using namespace std;
int main() {
	//creation of matricies
	int temp1[9] = {1,2,3,4,5,6,7,8,9};
	Matrix<int>* matrix1 = new Matrix<int>(3,3,temp1);
	Matrix<int>* matrix2 = new Matrix<int>(3,3,temp1);
	//test the overloaded + operator
	Matrix<int> sum = *matrix1 + *matrix2;
	cout<<"SUM:\n"<<sum<<endl;
	//test the overloaded - operator
	Matrix<int> substract = *matrix1 - *matrix2;
	cout<<"SUBSTRACT:\n"<<substract<<endl;
	//test the overloaded * operator
	Matrix<int> multiply = *matrix1 * *matrix2;
	cout <<"MULTIPLY \n"<< multiply <<endl;
	//test scalar multiplication
	Matrix<int> scalarMultiply = *matrix1 * 2;
	cout<<"SCALAR MULTIPLICATION\n" <<scalarMultiply<<endl;
	//test transpose matrix
	cout<<"TRANSPOSE\n"<<matrix1->transpose()<<endl;
	//test the overloaded == operator
	cout <<"== OPERATOR\n"<<(*matrix1 == *matrix2?"TRUE":"FALSE")<<endl;
	// test the Gaussian Elimination
	double temp3[16] = {2,4,-2,-2,1,2,4,-3,-3,-3,8,-2,-1,1,6,-3};
	Matrix<double>* matrix3 = new Matrix<double>(4,4,temp3);
	cout <<"GAUSSIAN ELIMINATION\n" << matrix3->gaussElimination()<<endl;
	// test the solve funciton
	double temp4[4] = {7,4,-7,1};
	Matrix<double>* matrix4 = new Matrix<double>(1,4,temp4);
	Matrix<double> solution = matrix3->solve(*matrix4);
	cout<<"SOLVE A MATRIX\n"<<solution<<endl;
	// test inverse matrix
	double temp5[9] = {2,1,4,3,2,1,-1,4,-2};
	Matrix<double>* matrix5 = new Matrix<double>(3,3, temp5);
	cout<<"INVERSE\n" << matrix5->inverse() <<endl;
}
