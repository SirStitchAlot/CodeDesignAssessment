#include "pch.h"
#include "CppUnitTest.h"
#include "../Project1/Critter.h"
#include "../Project1/Resource.h"
#include <stdio.h>
#include "iostream"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(MemoryUsuage)
		{
			Critter test;

			int newclass = sizeof(test);

			int oldclass = sizeof(int);

		std::cout<<newclass;






			//{with this test i will compare the old class to the new one
			// to do this i will use sizeof() which show how many bytes it takes up 
			// I will use an array with both classes so the difference is more obvious
			// I think i need to operator overload to test the difference 
			// 
			// 
			// }

		}
	};
}
