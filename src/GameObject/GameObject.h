#ifndef GAME_OBJECT
#pragma once

#include <vector>
#include <smath/smath.h>

class GameObject
{
public:
    // Constructor & Deconstructor
    GameObject(smath::transform2D transform = smath::transform2D(), GameObject* parent = nullptr)
    : mTransform(transform) { setParent(parent); }

    // Getters
    smath::transform2D* getTransform() { return &mTransform; }

    // Parenting functions
    void setParent(GameObject* gameObject);
    void removeParent();
    void removeParentKeepTransformation();

private:
    // Transform is required, all GameObjects will have one
    smath::transform2D mTransform;
    GameObject* mpParent = nullptr;
};


#endif // !GAME_OBJECT
