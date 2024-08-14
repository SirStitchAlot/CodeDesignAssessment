#include <iostream>
#include <stdio.h>
#include "raylib.h"
#include "chrono"



class testclass {
public:

	testclass() {};

	~testclass() {};

	
	Texture2D hahah;


};

int main() {

	using namespace std::chrono;

	steady_clock::time_point t1 = steady_clock::now();

	std::cout << "printing out 1000 stars...\n";
	for (int i = 0; i < 1000; ++i) std::cout << "*";
	std::cout << std::endl;

	steady_clock::time_point t2 = steady_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	testclass uhhh[50];

	int classsize = sizeof(uhhh);


	std::cout << "It took me " << time_span.count() << " seconds.";
	std::cout << std::endl;

	return 0;
	

	

	

	
	



}