#include "linear.cpp"

int main() {
	int templ[9] = {1,2,3,4,5,6,7,8,9};
	Matrix<int>* matrix1 = new Matrix<int>(3,3,templ);
	Matrix<int>* matrix2 = new Matrix<int>(3,3,templ);
	Matrix<int> sum = *matrix1-*matrix2;
	std::cout << sum <<std::endl;
}
