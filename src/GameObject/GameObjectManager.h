#ifndef GAMEOBJECT_MANHGER
#pragma once

#include <vector>
#include <smath/smath.h>
#include "GameObject.h"

class GameObjectManager
{
public:
    // Constructor & Deocnstructor
    GameObjectManager() {}
    ~GameObjectManager() { cleanup(); }

    // Data management functions
    bool init();
    void cleanup();

    // Getters
    int getObjectCount() const { return mpGameObjects.size(); }
    GameObject* getObjectAtIndex(const int& index);

    // Active Object functions
    GameObject* getActiveObject();
    void setActiveObject(const int& index);
    void resetActiveObject();

    // GameObject creation functions
    void createEmptyObject(smath::transform2D transform = smath::transform2D());
    void createDefaultObject(smath::transform2D transform = smath::transform2D());

    // GameObject vector functions
    void addGameObject(GameObject* gameObject);
    void removeObjectAt(const int& index);
    void removeAllObjects();

private:
    std::vector<GameObject*> mpGameObjects;
    int mActiveProjectIndex = -1;
};

#endif // !GAMEOBJECT_MANHGER
