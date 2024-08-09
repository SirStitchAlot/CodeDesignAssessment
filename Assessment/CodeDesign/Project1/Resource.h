#pragma once
#include <string>
#include <memory>

template <class T>
class Resource
{
/*=todo/notes===================================================================================*/

	//are Resource and Resource manager supposed ot be in the same file???
	//decent chance i get rid of this lmao 



/*=Structors==================================================================================================*/
public:
	Resource(const char *filename) {
		m_filename = filename;

		m_pData->LoadTexture(m_filename);
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
	
	const char* m_filename;

	std::unique_ptr<T> m_pData = Texture2D;

	


/*=methods======================================================================================================*/
public:
	
	std::unique_ptr<T> GetUnique_ptr() { return m_pData; }

	const char* getFilename() {return m_filename;}

/*=Funnies======================================================*/


};

