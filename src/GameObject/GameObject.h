#ifndef GAME_OBJECT
#pragma once

#include <vector>
#include "Components/Component.h"

class GameObject
{
public:
    // Constructor & Deconstructor
    GameObject()

private:
    std::vector<Component*> mpComponents;
};


#endif // !GAME_OBJECT
