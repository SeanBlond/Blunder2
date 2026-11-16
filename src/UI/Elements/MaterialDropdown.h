#pragma once
#include "../UIElement.h"

class MaterialManager;
class Material;

class MaterialDropdown : public UIElement
{
public:
    MaterialDropdown(UIQuad quad, GameObject* object, MaterialManager* manager, Event* clickEvent = nullptr)
        : mpIndex(index), mOptions(options), mpClickEvent(clickEvent), UIElement(quad, UI_DROPDOWN)
    {
        generateInteractable();
    }
    ~MaterialDropdown() { delete mpClickEvent; mpClickEvent = nullptr; }

    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void onHold(MouseClickData clickData) override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    GameObject* mpObject; // DOES NOT OWN, DO NOT DEALLOCATE
    std::vector<uint16_t> mMaterialOptions;
    MaterialManager* mpManager = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    bool mDroppedDown = false;
    Event* mpClickEvent;
};