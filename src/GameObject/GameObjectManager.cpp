#include "GameObjectManager.h"

// Data management functions
bool GameObjectManager::init()
{
    // TODO: Create default material stored in the manager

    // Adding a few game objects
    createEmptyObject(smath::transform2D(smath::vec2(-5, -2), 3, 0.0f, smath::vec2(1)));
    createEmptyObject(smath::transform2D(smath::vec2(5, 2), 1, 0.0f, smath::vec2(5, 10)));
    createEmptyObject(smath::transform2D(smath::vec2(-5, -2), 2, 0.0f, smath::vec2(3, 2)));

    // Setting active object index
    setActiveObject(0);

    // Return true for successful initialization
    return true;
}
void GameObjectManager::cleanup()
{
    // Deleting all game objects
    removeAllObjects();

    // TODO: Deallocate default material
}

// Getters
GameObject* GameObjectManager::getObjectAtIndex(const int& index)
{
    // If index is invalid, return null
    if (index < 0 || index > mpGameObjects.size())
        return nullptr;

    // Returning non-invalid game object
    return mpGameObjects[index];
}

// Active Object function definitions
GameObject* GameObjectManager::getActiveObject()
{
    // if the active object index isn't valid, return null
    if (mActiveProjectIndex < 0 || mActiveProjectIndex >= getObjectCount())
        return nullptr;

    // Returning the (valid) active object
    return mpGameObjects[mActiveProjectIndex];
}
void GameObjectManager::setActiveObject(const int& index)
{
    // If the entered index isn't valid, reset the active object
    if (index < 0 || index >= getObjectCount())
        resetActiveObject();
    else
        mActiveProjectIndex = index;
}
void GameObjectManager::resetActiveObject()
{
    // Setting active index to be -1
    mActiveProjectIndex = -1;
}

// GameObject creation functions
void GameObjectManager::createEmptyObject(smath::transform2D transform)
{
    // Creating a simple ahh game object
    GameObject* emptyObject = new GameObject(transform);

    // Adding the game object
    addGameObject(emptyObject);
}
void GameObjectManager::createDefaultObject(smath::transform2D transform)
{
    // Does nothing yet, need a material system
}

// GameObject vector functions
void GameObjectManager::addGameObject(GameObject* gameObject)
{
    // Ensuring the object isn't null
    if (!gameObject)
        return;

    // Pushing back the non-null object
    mpGameObjects.push_back(gameObject);
}
void GameObjectManager::removeObjectAt(const int& index)
{
    // If index is invalid, return out
    if (index < 0 || index > mpGameObjects.size())
        return;

    // Removing the indexed object
    mpGameObjects.erase(mpGameObjects.begin() + index);
}
void GameObjectManager::removeAllObjects()
{
    for (GameObject* object : mpGameObjects)
    {
        delete object;
        object = nullptr;
    }
    mpGameObjects.clear();
}