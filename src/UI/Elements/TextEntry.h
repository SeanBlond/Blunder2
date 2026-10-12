#ifndef TEXT_ENTRY
#pragma once

#include "../UIElement.h"

class TextEntry : public UIElement, public TextInput
{
public:
    TextEntry(UIQuad quad, std::string* value)
        : mpValue(value), UIElement(quad, UI_TEXT_ENTRY), TextInput(*value) { setValue(*mpValue); generateInteractable(); }

    // Element Functions
    void setValue(const std::string& value) { (*mpValue) = value; }
    void setValue(std::string* value) { this->mpValue = value; }

    // Override functions
    void generateInteractable() override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;
    TextInput* getTextInput() override { return this; }
    void handleInput() override;

private:
    std::string* mpValue;
};

#endif // !TEXT_ENTRY
