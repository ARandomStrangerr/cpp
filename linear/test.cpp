#include "linear.cpp"
using namespace std;
int main() {
	//creation of matricies
	int templ[9] = {1,2,3,4,5,6,7,8,9};
	Matrix<int>* matrix1 = new Matrix<int>(3,3,templ);
	Matrix<int>* matrix2 = new Matrix<int>(3,3,templ);
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
	cout<<"TRANSPOSE\n"<<matrix1->tranpose()<<endl;
	//test the overloaded == operator
	cout <<"== OPERATOR\n"<<(*matrix1 == *matrix2?"TRUE":"FALSE")<<endl;
}
