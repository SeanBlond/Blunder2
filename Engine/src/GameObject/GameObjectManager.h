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
    GameObject* getActiveObject() { return mpActiveObject; }
    GameObject* getRootObject() { return mpRootObject; }

    // Active Object Setters
    void setActiveObject(GameObject* object);
    void resetActiveObject();

    // GameObject creation functions
    GameObject* createEmptyObject(std::string name, smath::transform2D transform = smath::transform2D());
    GameObject* createDefaultObject(std::string name, smath::transform2D transform = smath::transform2D());

    // GameObject vector functions
    void addGameObject(GameObject* gameObject);
    void removeAllObjects();

private:
    GameObject* mpRootObject = nullptr;
    GameObject* mpActiveObject = nullptr;
};

#endif // !GAMEOBJECT_MANHGER
