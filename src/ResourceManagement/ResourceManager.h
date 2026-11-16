#ifndef RESOURCE_MANAGER
#pragma once

#include <string>
#include "MaterialManager.h"

// Class that contains the underlying architecture and software loop (singleton)
class ResourceManager
{
public:
	// Singleton Instance Functions
	static ResourceManager* createInstance();
	static ResourceManager* getInstance();
	static void destroyInstance();

	// Management Functions
	bool init();
	void cleanup();

	// Manager Getters
	MaterialManager* getMaterialManager() { return &mpMaterialManager; }

private:
	// Private Constructor & Deconstructor
	ResourceManager() {}
	~ResourceManager() { cleanup(); }

	// Singleton Instance
	static ResourceManager* mpInstance;

	// Various Data Manager pointers
	MaterialManager mpMaterialManager;

	// Filepath constants
	const std::string MATERIALS_FILEPATH = "resources/Materials/materials.txt";
};

#endif // !RESOURCE_MANAGER
