#include <iostream>
#include <stdio.h>




class testclass {
public:

	testclass() {};

	~testclass() {};


	int lmao;

	char lol;


};

int main() {


	testclass uhhh;

	int classsize = sizeof(uhhh);


	if (classsize<20)
	{
		std::cout << "its true";
	}
	else {
		std::cout << "its false";

	}

	




}