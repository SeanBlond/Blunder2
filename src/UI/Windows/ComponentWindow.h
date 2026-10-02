#ifndef COMPONENT_WINDOW
#pragma once

#include "../UIWindows.h"
#include "../../GameObject/GameObject.h"

struct ComponentUIGroup
{
    ComponentUIGroup(std::string name, bool droppedDown = true)
        : mName(name), mDroppedDown(droppedDown) {}
    ~ComponentUIGroup()
    {
        // Deleting each element
        for (UIElement* element : mpElements)
        {
            delete element; 
            element = nullptr;
        }
        mpElements.clear();
    }

    std::string mName;
    std::vector<UIElement*> mpElements;
    UIQuad mQuad = UIQuad();
    bool mDroppedDown;
};

class ComponentWindow : public UIWindow
{
public:
    // Constructor(s)
    ComponentWindow(GameObject* gameObject, float width, float height, float xoffset, float yoffset)
        : mpGameObject(gameObject), UIWindow(width, height, xoffset, yoffset, "Components") {}
    ComponentWindow(GameObject* gameObject)
        : mpGameObject(gameObject), UIWindow(0, 0, 0, 0, "Components") {}
    ~ComponentWindow() { removeAllComponentGroups();  }

    // Component & Element related functions (some overrides)
    void removeAllComponentGroups();
    void addComponentGroup(ComponentUIGroup* componentGroup);
    void generateComponentGroups();
    void updateComponentGroupPositions();
    UIElement* getElementAtPos(smath::ivec2 position) override;

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

private:
    GameObject* mpGameObject = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    std::vector<ComponentUIGroup*> mpComponentGroups;
    int mTestInt = 0;
    int mTestClampInt = 0;
};

#endif // !COMPONENT_WINDOW
