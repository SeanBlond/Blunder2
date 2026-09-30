#include "UIWIndows.h"

// Getters / Accessors
UIElement* UIWindow::getElementAtPos(smath::ivec2 position)
{
    // Looping through each element to check for collisions
    for (UIElement* element : mpElements)
    {
        if (element->checkCollision(position))
        {
            return element;
        }
    }

    // No collision detected, return null
    return nullptr;
}


// Setters / Modifiers
void UIWindow::addElement(UIElement* element)
{
    mpElements.push_back(element);
}
void UIWindow::removeElementAt(int index)
{
    if (index < 0 && index >= mpElements.size()) 
        return;

    delete mpElements[index]; 
    mpElements[index] = nullptr; 
    mpElements.erase(mpElements.begin() + index);
}
void UIWindow::removeAllElements()
{ 
    for (int i = 0; i < mpElements.size(); i++) 
    { 
        delete mpElements[i]; 
        mpElements[i] = nullptr; 
    } 
    mpElements.clear(); 
}