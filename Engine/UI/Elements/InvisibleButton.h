#pragma once
#include "../UIElement.h"

class InvisibleButton : public UIElement
{
public:
    InvisibleButton(const UIQuad& quad, Event* clickEvent)
        : mClickEvent(clickEvent), UIElement(quad, UI_BUTTON) {
        generateInteractable();
    }
    ~InvisibleButton() { delete mClickEvent; mClickEvent = nullptr; }
    
    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    Event* mClickEvent;
};