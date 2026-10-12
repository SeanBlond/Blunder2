#ifndef HIERARCHY_WINDOW
#pragma once

#include "../UIWindows.h"
#include "../../GameObject/GameObject.h"

struct HierarchyUIGroup
{
    HierarchyUIGroup(GameObject* object) : mpObject(object) {}
    ~HierarchyUIGroup()
    {
        // Deleting each element
        for (UIElement* element : mpElements)
        {
            delete element;
            element = nullptr;
        }
        mpElements.clear();
    }

    bool getDroppedDown() { return (mpObject ? mpObject->getObjectUIData()->mDroppedDown : false); }

    GameObject* mpObject = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    std::vector<UIElement*> mpElements;
    UIQuad mQuad = UIQuad();
};

class HierarchyWindow : public UIWindow, public EventListener
{
public:
    // Constructor(s)
    HierarchyWindow(GameObject* rootObject, float width, float height, float xoffset, float yoffset)
        : mpRootObject(rootObject), UIWindow(width, height, xoffset, yoffset, "Hierarchy") {}
    HierarchyWindow(GameObject* rootObject)
        : mpRootObject(rootObject), UIWindow(0, 0, 0, 0, "Hierarchy") {}
    ~HierarchyWindow() { removeAllHierarchyGroups(); }

    // HierarchyGroup & Element related functions (some overrides)
    void removeAllHierarchyGroups();
    void addGroupFromObject(GameObject* object, float uiScale, float& yPos, int indent);
    void generateHierarchyGroups();
    void updateHierarchyGroupPositions();
    UIElement* getElementAtPos(smath::ivec2 position) override;

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

    // Override functions (EventListener)
    void handleEvent(const Event& event) override;

private:
    GameObject* mpRootObject = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    std::vector<HierarchyUIGroup*> mpHierarchyGroups;
    const float INDENT_SCALE = 20.0f;
};

#endif // !HIERARCHY_WINDOW
