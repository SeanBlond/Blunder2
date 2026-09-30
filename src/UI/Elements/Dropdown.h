#pragma once
#include "../UIElement.h"

class Dropdown : public UIElement
{
public:
    Dropdown(UIQuad quad, int* index, const std::vector<std::string>& options, Event* clickEvent = nullptr)
        : mpIndex(index), mOptions(options), mpClickEvent(clickEvent), UIElement(quad, UI_BUTTON)
    {
        generateInteractable();
    }
    ~Dropdown() { delete mpClickEvent; mpClickEvent = nullptr; }

    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void onHold(MouseClickData clickData) override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    int* mpIndex;
    std::vector<std::string> mOptions;
    bool mDroppedDown = false;
    Event* mpClickEvent;
};