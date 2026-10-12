#ifndef GAME_OBJECT
#pragma once

#include <vector>
#include <smath/smath.h>

struct ObjectUIData
{
    bool mVisible = true;
    bool mSelectable = true;
    bool mDroppedDown = true;
};

class GameObject
{
public:
    // Constructor & Deconstructor
    GameObject(std::string name, smath::transform2D transform = smath::transform2D())
    : mName(name), mTransform(transform) {}
    ~GameObject();

    // Getters
    smath::transform2D* getTransform() { return &mTransform; }
    std::string getName() const { return mName; }
    int getChildrenCount() const { return (int)(mpChildren.size()); }
    GameObject* getChildAtIndex(const int& index);

    // Parenting functions
    void changeParent(GameObject* gameObject);

    // Children functions
    void addChild(GameObject* childObject);
    void removeChildAtIndex(const int& index);
    void removeChildFromPointer(GameObject* childPointer);
    void removeAllChildren();

private:
    // Transform is required, all GameObjects will have one
    smath::transform2D mTransform;
    std::string mName = "";
    GameObject* mpParent = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    std::vector<GameObject*> mpChildren; // feel free to deallocate these tho
};


#endif // !GAME_OBJECT
