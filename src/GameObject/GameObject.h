#ifndef GAME_OBJECT
#pragma once

#include <vector>
#include <smath/smath.h>

class GameObject
{
public:
    // Constructor & Deconstructor
    GameObject() {}

    // Getters
    smath::transform2D* getTransform() { return &mTransform; }

private:
    // Transform is required, all GameObjects will have one
    smath::transform2D mTransform;
};


#endif // !GAME_OBJECT
