#include "GameObject.h"

// Deconstructor definition
GameObject::~GameObject()
{
    // Deallocating all children
    removeAllChildren();
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

// Parenting function definitions
void GameObject::changeParent(GameObject* gameObject)
{
    // If the new parent is the same as the old, return out
    if (mpParent == gameObject)
        return;

    // If the current parent is valid, remove this object as a child
    if (mpParent)
        mpParent->removeChildFromPointer(this);

    // Setting new parent
    mpParent = gameObject;

    // If the parent isn't null, add this object as a child
    if (mpParent)
        mpParent->addChild(this);
}

// Children function definitions
void GameObject::addChild(GameObject* childObject)
{
    // If the object isn't null, set its parent and add it to the vector
    if (childObject)
    {
        mpChildren.push_back(childObject);
    }
}
void GameObject::removeChildAtIndex(const int& index)
{
    // If index is invalid, return out
    if (index < 0 || index >= mpChildren.size())
        return;

    // Setting the parent to be null
    mpChildren[index]->changeParent(nullptr);

    // Deleting the child
    delete mpChildren[index];
    mpChildren[index] = nullptr;

    // Erasing the child from the vector
    mpChildren.erase(mpChildren.begin() + index);
}
void GameObject::removeChildFromPointer(GameObject* childPointer)
{
    // Searching the vector for children
    for (int i = 0; i < mpChildren.size(); i++)
    {
        // Checking if the child exists in the vector
        if (mpChildren[i] == childPointer)
        {
            // Deleting the child
            delete mpChildren[i];
            mpChildren[i] = nullptr;

            // Erasing the child from the vector
            mpChildren.erase(mpChildren.begin() + i);
        }
    }
}
void GameObject::removeAllChildren()
{
    // Looping through each child and removing it
    for (GameObject* object : mpChildren)
    {
        // Deleting the child
        delete object;
        object = nullptr;
    }
    mpChildren.clear();
}