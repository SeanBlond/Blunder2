#pragma once
#include "../UIElement.h"

class TextButton : public UIElement
{
public:
    TextButton(UIQuad quad, std::string label, Event* clickEvent, float size, Color color = Color(1))
        : mLabel(label), mClickEvent(clickEvent), mLabelSize(size), mLabelColor(color), UIElement(quad, UI_BUTTON)
    { generateInteractable(); }
    ~TextButton() { delete mClickEvent; mClickEvent = nullptr; }
    // Getters
    std::string getLabel() const { return mLabel; }

    // Setters
    void setLabel(const std::string& text) { mLabel = text; }

    // Override functions
    void generateInteractable() override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    std::string mLabel;
    float mLabelSize;
    Color mLabelColor;
    Event* mClickEvent;
};