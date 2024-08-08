#pragma once
#include <string>
#include <memory>

template <class T>
class Resource
{
/*=todo/notes===================================================================================*/

	//are Resource and Resource manager supposed ot be in the same file???



/*=Structors==================================================================================================*/
public:
	Resource(const std::string& filename) {
		m_filename = filename;
	}

	~Resource() {
		if (T!=nullptr)
		{
			delete[] T;
			T = nullptr;
		}
	
	}


/*=Properties==================================================================================================================*/
private:
	
	const char m_filename [20];

	std::unique_ptr<T> m_pData; ///what is this used for 


/*=methods======================================================================================================*/
public:
	const char getFilename() {return m_filename;}


/*=Funnies======================================================*/


};

