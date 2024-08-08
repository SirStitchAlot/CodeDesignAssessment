#include <vector>
#include <memory>
#include "raylib.h"
#include "Resource.h"

template <class T> //????

class ResourceManager
{
/*=todo/notes===================================================================================*/
//figure out if i need a second class for this lmao 

/*=Structors==================================================================================================*/
public:
	//resource managers shouldn't be copeid
	ResourceManager(const ResourceManager& other) = delete; //I seriously dont get the point of these???

	ResourceManager(ResourceManager&& other) = delete;

	ResourceManager& operator=(const ResourceManager& other) = delete;

	~ResourceManager(){
		if (T != nullptr) {
			delete[] T;
			T = nullptr;
		}
	}

/*=Properties==================================================================================================================*/
private:
	std::vector<std::shared_ptr<Resource<T>>> m_resources; //might have to figure out if i need to move the template
	
/*=methods======================================================================================================*/
public:
	std::shared_ptr < Resource<T>> get(const std::string&) {}  

	void collectGarbage() {}

	int getCount() { m_resources.get_count();}
/*=Funnies======================================================*/
//hahahahahahaha
};

