#pragma once
#include "../../UIElement.h"

class Material;

class MaterialDropdown : public UIElement
{
public:
    MaterialDropdown(UIQuad quad, GameObject* object, Event* clickEvent = nullptr)
        : mpObject(object), mpClickEvent(clickEvent), UIElement(quad, UI_DROPDOWN)
    {
        generateInteractable();
        storeMaterialName();
    }
    ~MaterialDropdown() { delete mpClickEvent; mpClickEvent = nullptr; }

    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void onHold(MouseClickData clickData) override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;


private:
    // Private stuct for containing dropdown option info
    struct DropdownInfo
    {
        uint16_t mKey;
        std::string mName;
    };

    GameObject* mpObject = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
    std::string mStoredMaterialName = "";
    Event* mpClickEvent;

    // Interaction Elements
    UIQuad mInteractableQuad;
    std::vector<DropdownInfo> mMaterialOptions;
    int mIndex = 0;
    bool mDroppedDown = false;

    // Private Function for defining and storing the game object's material's name
    void storeMaterialName();
};