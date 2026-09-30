#ifndef UI_WINDOWS
#pragma once

#include "UIElement.h"
#include "Elements/UIElements.h"
#include "../Runtime/Time.h"

class UIWindow
{
public:
    // Constructor & Deconstructor
    UIWindow(float width, float height, float xoffset, float yoffset, std::string name)
        : mPosition(width, height, xoffset, yoffset), mWindowName(name) {}
    UIWindow(WindowPosition position, std::string name)
        : mPosition(position), mWindowName(name) {}
    virtual ~UIWindow() { removeAllElemenets(); }

    // Getters
    UIElement* getElement(int index) { return mpElements[index]; }
    UIElement* getElementAtPos(smath::ivec2 position);
    std::string getWindowName() { return mWindowName; }

    // Setters / Modifiers
    void addElement(UIElement* element);
    void removeElementAt(int index);
    void removeAllElemenets();

    // Pure Virtual Functions
    virtual void OpenWindow() = 0;
    virtual void ResizeWindow() = 0;
    virtual void CloseWindow() = 0;
    virtual void DrawWindow(GraphicsRenderer* renderer) = 0;

    // Public member variables
    WindowPosition mPosition;

protected:
    //GraphicsBuffer* mpWindowBuffer;
    std::string mWindowName;
    std::vector<UIElement*> mpElements;
};


#endif // !UI_WINDOWS
