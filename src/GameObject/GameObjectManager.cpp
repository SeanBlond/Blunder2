#include "GameObjectManager.h"

// Data management functions
bool GameObjectManager::init()
{
    // Setting the root object to be null
    mpRootObject = new GameObject("ROOT");

    // TODO: Create default material stored in the manager

    // Adding a few game objects
    GameObject* newObject = createEmptyObject("Test 1", smath::transform2D(smath::vec2(-5, -2), 3, 0.0f, smath::vec2(1)));
    createEmptyObject("Test 2", smath::transform2D(smath::vec2(5, 2), 1, 0.0f, smath::vec2(5, 10)));
    createEmptyObject("Test 3", smath::transform2D(smath::vec2(-5, -2), 2, 0.0f, smath::vec2(3, 2)));

    // Setting active object index
    setActiveObject(newObject);

    // Return true for successful initialization
    return true;
}
void GameObjectManager::cleanup()
{
    // Deleting all game objects
    removeAllObjects();

    // TODO: Deallocate default material
}


// Active Object Setters
void GameObjectManager::setActiveObject(GameObject* object)
{
    // Checking if new object is null
    if (object == nullptr)
        resetActiveObject();

    // Otherwise, set the active object
    mpActiveObject = object;
}
void GameObjectManager::resetActiveObject()
{
    // Setting the active object to be the root object
    mpActiveObject = mpRootObject;
}

// GameObject creation functions
GameObject* GameObjectManager::createEmptyObject(std::string name, smath::transform2D transform)
{
    // Creating a simple ahh game object
    GameObject* emptyObject = new GameObject(name, transform);

    // Adding the game object
    addGameObject(emptyObject);

    return emptyObject;
}
GameObject* GameObjectManager::createDefaultObject(std::string name, smath::transform2D transform)
{
    // Does nothing yet, need a material system
    return nullptr;
}

// GameObject vector functions
void GameObjectManager::addGameObject(GameObject* gameObject)
{
    // Ensuring the object isn't null
    if (!gameObject || !mpRootObject)
        return;

    // Pushing back the non-null object
    gameObject->setParent(mpRootObject);
}
void GameObjectManager::removeAllObjects()
{
    delete mpRootObject;
    mpRootObject = nullptr;
}