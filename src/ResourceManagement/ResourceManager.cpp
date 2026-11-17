#include "ResourceManager.h"

// Setting the mpInstance to be null
ResourceManager* ResourceManager::mpInstance = nullptr;

// Singleton Instance Functions Definitions
ResourceManager* ResourceManager::createInstance()
{
    // If the instance already exists, destroy it so it can be recreated
    if (mpInstance)
        destroyInstance();

    // Creating the instance
    mpInstance = new ResourceManager();

    // Returning the instance
    return mpInstance;
}
ResourceManager* ResourceManager::getInstance()
{
    // Creating the instance if it's not already
    if (!mpInstance)
        createInstance();

    // Returning the instance
    return mpInstance;
}
void ResourceManager::destroyInstance()
{
    // Deleting the instance and setting it to null
    delete mpInstance;
    mpInstance = nullptr;
}

// Management Functions
bool ResourceManager::init()
{
    // Initiailzaing the various managers
    bool successfulInit = true;
    successfulInit &= mMaterialManager.init(MATERIALS_FILEPATH);

    // Returning the outcome of the initializations
    return successfulInit;
}
void ResourceManager::cleanup()
{
    // Does nothing yet
}

// Material Manager Function definitions
Material* ResourceManager::getMaterial(uint16_t materialID)
{
    return mMaterialManager.getMaterial(materialID);
}
SDL_GPUGraphicsPipeline* ResourceManager::getBasicMaterialPipeline()
{
    return mMaterialManager.getBasicPipeline();
}
bool ResourceManager::addMaterial(Material* material)
{
    return mMaterialManager.addMaterial(material);
}
void ResourceManager::removeMaterial(uint16_t materialID)
{
    mMaterialManager.removeMaterial(materialID);
}