#ifndef LABELLED_TEXT_ENTRY
#pragma once

#include "../../UIElement.h"

class LabelledTextEntry : public UIElement, public TextInput
{
public:
    LabelledTextEntry(UIQuad quad, const std::string& label, std::string* value)
        : mLabel(label), mpValue(value), UIElement(quad, UI_TEXT_ENTRY), TextInput(*value) {
        setValue(*mpValue); generateInteractable();
    }

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
    std::string* mpValue = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    std::string mLabel = "";
    UIQuad mInteractableQuad;
};

#endif // !LABELLED_TEXT_ENTRY
