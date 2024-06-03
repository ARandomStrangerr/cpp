#include "SingleLinkList.h"

int main () {
	int v1=1,v2=2,v3=3,v4=4,v5=5,v6=6,v7=7,v8=8,v9=9;
	SingleLinkList<int> lst;
	lst.insertFirst(v5);
	lst.insertFirst(v1);
	lst.insertLast(v9);
	std::cout << "removed element " << lst.removeFirst()  << std::endl;
	std::cout<<lst<<std::endl;
}

