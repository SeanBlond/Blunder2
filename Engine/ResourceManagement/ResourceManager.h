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

	// Enum for resource types
	enum ResourceType { RESOURCE_MATERIAL, RESOURCE_BEHAVIOUR, RESOURCE_AUDIO };

	// Management Functions
	bool init();
	void cleanup();

	// Material Manager Functions
	Material* getMaterial(uint16_t materialID);
	SDL_GPUGraphicsPipeline* getBasicMaterialPipeline();
	bool addMaterial(Material* material);
	void removeMaterial(uint16_t materialID);
	MaterialManager* getMaterialManager() { return &mMaterialManager; }

private:
	// Private Constructor & Deconstructor
	ResourceManager() {}
	~ResourceManager() { cleanup(); }

	// Singleton Instance
	static ResourceManager* mpInstance;

	// Various Data Manager pointers
	MaterialManager mMaterialManager;

	// Filepath constants
	const std::string mProjectRoot = "../TestProject/";
};

#endif // !RESOURCE_MANAGER
