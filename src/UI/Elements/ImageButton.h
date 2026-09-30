#pragma once
#include "../UIElement.h"

class ImageButton : public UIElement
{
public:
    ImageButton(UIQuad quad, std::string imageKey, Event* clickEvent)
        : mImageKey(imageKey), mClickEvent(clickEvent), UIElement(quad, UI_BUTTON) { generateInteractable(); }
    ~ImageButton() { delete mClickEvent; mClickEvent = nullptr; }
    // Getters
    std::string getImageKey() const { return mImageKey; }

    // Setters
    void setImageKey(const std::string& key) { mImageKey = key; }

    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    std::string mImageKey;
    Event* mClickEvent;
};