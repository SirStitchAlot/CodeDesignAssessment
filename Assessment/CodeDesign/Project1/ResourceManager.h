#include <vector>
#include <memory>
#include "raylib.h"
#include "Resource.h"

template <class T> //????

class ResourceManager
{
/*=todo/notes=================================================================================================*/
//figure out if i need a second class for this lmao 

/*=Structors==================================================================================================*/
public:

	ResourceManager() {}; //example one has the constructor as private

	~ResourceManager(){
		if (T != nullptr) {				 //idk if this is needed Lmao
			delete[] T;					 //idk if this is needed Lmao
			T = nullptr;				 //idk if this is needed Lmao
		}
	}

/*=Properties====================================================================================================*/
private:
	//keeps track of the resouces
	std::vector<std::shared_ptr<Resource<T>>> m_resources; 
	
/*=methods======================================================================================================*/
public:

	void load(const char * filename) {

		auto& resourceManager = instance();
		std::shared_ptr<T> resource;

		//this is where we do checks if the resouce has been loaded already 

		auto resIter = resourceManager.resources.find(filename);   //not entirely sure what this iter is doing

		if (resIter != resourceManager.resources.end()) {
			//the resource was loaded alreadly 
			resource = resIter->second.lock();  //wth is this doing lmao!!!!!
		}

		if (!resource) {

			resource = std::make_shared<T>(filename);
			ResourceManager.resources[filename] = resource;
		}



	}

	std::shared_ptr < Resource<T>> get(const char * texture) {
		
	}  

	void collectGarbage() {} //yeah idk what this is doing

	int getCount() {m_resources.size();}
/*=Funnies=====================================================================================================*/
	//resource managers shouldn't be copeid
	ResourceManager(const ResourceManager& other) = delete;

	ResourceManager(ResourceManager&& other) = delete;

	ResourceManager& operator=(const ResourceManager& other) = delete;
};

