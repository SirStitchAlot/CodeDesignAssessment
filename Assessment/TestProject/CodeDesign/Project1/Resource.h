#pragma once
#include <string>
#include <memory>
#include <raylib.h>


class Resource
{
/*=todo/notes===================================================================================*/

	//are Resource and Resource manager supposed ot be in the same file???
	//decent chance i get rid of this lmao 
	//if the texture doesnt load properly try adding raymath 


/*=Structors==================================================================================================*/
public:
	Resource(const char *filename) {
		m_filename = filename;

		m_pData = LoadTexture(m_filename);
	}

	~Resource() {
		
		if (m_filename !=nullptr)
		{
			m_filename = nullptr;
		}
	}


/*=Properties==================================================================================================================*/
private:
	
	const char* m_filename;

	Texture2D m_pData;

	


/*=methods======================================================================================================*/
public:
	
	Texture2D GetTexture() { return m_pData; }

	const char* getFilename() {return m_filename;}

/*=Funnies======================================================*/
    


};

