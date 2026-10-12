#ifndef TEXT_ENTRY
#pragma once

#include "../UIElement.h"

class GameObject;
class HierarchyTextEntry : public UIElement, public TextInput
{
public:
    HierarchyTextEntry(UIQuad quad, std::string* value, GameObject* refObject)
        : mpValue(value), mpRefObject(refObject), UIElement(quad, UI_TEXT_ENTRY), TextInput(*value) { setValue(*mpValue); generateInteractable(); }

    // Element Functions
    void setValue(const std::string& value) { (*mpValue) = value; }
    void setValue(std::string* value) { this->mpValue = value; }

    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;
    TextInput* getTextInput() override { return this; }
    void handleInput() override;

private:
    std::string* mpValue = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    GameObject* mpRefObject = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    float mFirstClickTime = -1.0f;
    inline static float msDoubleClickLimit = 0.5f; // Allows for a 0.5 second delay between double clicks
    bool mDoubleClicked = false;
};

#endif // !TEXT_ENTRY
