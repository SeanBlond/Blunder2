#include "GameObject.h"

// Parenting function defintiions
void GameObject::setParent(GameObject* gameObject)
{
    // Setting the parent of the game object
    mpParent = gameObject;

    // Setting the parent of the transform 
    if (!gameObject)
        mTransform.parent = nullptr;
    else
        mTransform.parent = gameObject->getTransform();
}
void GameObject::removeParent()
{
    // Setting parent to be null
    setParent(nullptr);
}
void GameObject::removeParentKeepTransformation()
{
    // Does nothing yet
}