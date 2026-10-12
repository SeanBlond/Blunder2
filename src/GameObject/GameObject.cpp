#include "GameObject.h"

// Deconstructor definition
GameObject::~GameObject()
{
    // Deallocating all children
    deleteAllChildren();
}

// Getter definitions
GameObject* GameObject::getChildAtIndex(const int& index)
{
    // If index is invalid, return null
    if (index < 0 || index >= mpChildren.size())
        return nullptr;

    // Otherwise, return child
    return mpChildren[index];
}

void GameObject::setParent(GameObject* parent)
{
    if (this->mpParent != nullptr)
        removeParent();

    this->mpParent = parent;
    parent->addChild(this);

    if (parent)
        mTransform.parent = parent->getTransform();
}
void GameObject::addChild(GameObject* child)
{
    mpChildren.push_back(child);
}
void GameObject::removeParent()
{
    mpParent->removeChild(this);
    mpParent = nullptr;
}
void GameObject::removeChild(GameObject* child)
{
    int childIndex = findChild(child);

    if (childIndex == -1)
        return;
    else
    {
        mpChildren.erase(mpChildren.begin() + childIndex);
    }
}
int GameObject::findChild(GameObject* child)
{
    for (int i = 0; i < mpChildren.size(); i++)
    {
        if (mpChildren[i] == child)
            return i;
    }
    return -1;
}
void GameObject::deleteAllChildren()
{
    for (GameObject* object : mpChildren)
    {
        delete object;
        object = nullptr;
    }
    mpChildren.clear();
}