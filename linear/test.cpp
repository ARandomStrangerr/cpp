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
	cout<<"TRANSPOSE\n"<<matrix1->transpose()<<endl;
	//test the overloaded == operator
	cout <<"== OPERATOR\n"<<(*matrix1 == *matrix2?"TRUE":"FALSE")<<endl;
	cout <<"GAUSSIAN ELIMINATION" << endl;
	double temp2[16] = {2,4,-2,-2,1,2,4,-3,-3,-3,8,-2,-1,1,6,-3};
	Matrix<double>* matrix3 = new Matrix<double>(4,4,temp2);
	cout <<matrix3->gaussElimination()<<endl;
	cout <<"LU DECOMPOSITION"<<endl;
	cout<<matrix3->luDecomposition()<<endl;
}
