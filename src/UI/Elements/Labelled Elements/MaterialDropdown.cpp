#include "MaterialDropdown.h"
#include "../../../Material/Material.h"
#include "../../../ResourceManagement/ResourceManager.h"

// Override functions
void MaterialDropdown::generateInteractable()
{
    // Delete the old interactable
    delete mpInteractable;

    // Defining interactaqble quad
    mInteractableQuad = UIQuad(
        mQuad.x + mQuad.w * 0.3f,
        mQuad.y,
        mQuad.w * 0.7f,
        mQuad.h
    );

    // Creating new interactable
    mpInteractable = new QuadInteractable(mInteractableQuad.getSDL_FRect());
}

void MaterialDropdown::onClick(MouseClickData clickData)
{
    // Clearing the options list (just in case it isn't already)
    mMaterialOptions.clear();

    // Attempting to get a pointer to the Material Managers
    MaterialManager* manager = ResourceManager::getInstance()->getMaterialManager();
    if (!manager) { return; } // No manager, return out

    // Generating the material options
    for (auto materialOption : *(manager->getMaterialMap()))
    {
        mMaterialOptions.push_back({
            materialOption.first,
            materialOption.second->getName()
        });
    }

    // Activating dropdown
    mDroppedDown = true;

    // Clamping value to bhe within the options range
    mIndex = smath::clamp(mIndex, 0, (int)mMaterialOptions.size());
}
void MaterialDropdown::onHold(MouseClickData clickData)
{
    float lowestPoint = mQuad.y + mQuad.h + (mQuad.h * mMaterialOptions.size());
    bool directionDown = (lowestPoint < GraphicsDevice::getInstance()->getDisplayHeight());

    // Checking each options for a mouse y-collision
    for (int i = 0; i < mMaterialOptions.size(); i++)
    {
        // Getting the y positions to check
        float optionHeight = mQuad.h;
        float firstOptionYPos = mQuad.y + optionHeight;
        smath::vec2 optionCorners;

        if (directionDown)
        {
            optionCorners = smath::vec2(
                mQuad.y + mQuad.h,
                mQuad.y + (mQuad.h * (i + 2))
            );
        }
        else
        {
            optionCorners = smath::vec2(
                mQuad.y - (mQuad.h * (i + 1)),
                mQuad.y
            );
        }

        // Checking collision
        if (clickData.mPosition.y >= optionCorners.x && clickData.mPosition.y <= optionCorners.y)
        {
            mIndex = i;
            break;
        }
    }
}
void MaterialDropdown::onRelease(MouseClickData clickData)
{
    // Deactivating dropdown
    mDroppedDown = false;

    // Updating the gameobject material based on the selected option
    if (mpObject)
    {
        mpObject->setMaterialKey(mMaterialOptions[mIndex].mKey);
    }

    // Storing the newly selected material name
    storeMaterialName();

    // Clearing the options list
    mMaterialOptions.clear();

    // Calling click event if needed
    if (mpClickEvent)
        EventSystem::getInstance()->fire(*mpClickEvent);
}
void MaterialDropdown::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
    // Getting the color effetc
    Color effect = 1.0f;
    if (mClicked)
        effect = 0.5f;
    else if (mHighlighted)
        effect = 0.75f;

    // Drawing the label
    renderer->addText(
        "Material",
        smath::vec2(mInteractableQuad.x - renderer->getUIScale() * 4, mQuad.getCenterY()), layerOffset + 0.01f, renderer->getTextSize(TEXT_NORMAL),
        Color(1), TEXT_H_RIGHT, TEXT_V_MIDDLE);

    // Drawing Dropdown Box
    renderer->addRectangle(mInteractableQuad.getSDL_FRect(), layerOffset, Color(0.15f) * effect);

    // Drawing text for the current value
    renderer->addText(mStoredMaterialName, mInteractableQuad.MiddleRight() - smath::vec2(1.0f * renderer->getUIScale(), 0.0f), layerOffset + 0.01f, renderer->getTextSize(TEXT_NORMAL), Color(1.0f), TEXT_H_RIGHT, TEXT_V_MIDDLE);

    // Drawing Dropdown Icon
    std::string dropdownKey = (mDroppedDown ? "Dropdown_True_Symbol" : "Dropdown_False_Symbol");
    renderer->addTexQuad(mInteractableQuad.MiddleLeft() + smath::vec2(8.0f * renderer->getUIScale(), 0.0f), smath::vec2(mInteractableQuad.h * 0.5f), layerOffset + 0.02f, dropdownKey, Color(1), true);

    // Drawing each option if dropped down
    if (mDroppedDown)
    {
        // Storing the old renderer layer 
        RenderLayer oldLayer = renderer->getCurrentLayer();

        // Setting new layer 
        renderer->setActiveLayer(UI_POPUP_LAYER);

        // Setting optionYSize for UI interaction
        float optionWidth = mInteractableQuad.w;

        // Checking if there is enough space for the dropdown to drop downwards
        float lowestPoint = mInteractableQuad.y + mInteractableQuad.h + (mInteractableQuad.h * mMaterialOptions.size());
        bool directionDown = (lowestPoint < GraphicsDevice::getInstance()->getDisplayHeight());

        // Drawing a quad that outlines the options
        SDL_FRect outlineCorner =
        {
            mInteractableQuad.x - (renderer->getUIScale() * 5.0f),
            mInteractableQuad.y + (directionDown ? mInteractableQuad.h : -(mInteractableQuad.h * mMaterialOptions.size()) - (renderer->getUIScale() * 5.0f)),
            mInteractableQuad.w + (renderer->getUIScale() * 10.0f),
            (mInteractableQuad.h * mMaterialOptions.size()) + (renderer->getUIScale() * 5.0f)
        };
        renderer->addRectangle(outlineCorner, 0.0f, Color(0.25f));

        // Drawing each option
        for (int i = 0; i < mMaterialOptions.size(); i++)
        {
            float optionYOffset = mInteractableQuad.y + (directionDown ? 1 : -1) * (mInteractableQuad.h * (i + 1));

            // Highlighting the option if it is currently selected
            Color optionColor = (i % 2 == 0 ? Color(0.225f) : Color(0.2f));
            optionColor = (i == mIndex ? Color(0.275f) : optionColor);

            // Drawing Dropdown Box
            renderer->addRectangle(
                {
                    mInteractableQuad.x,
                    optionYOffset,
                    mInteractableQuad.w,
                    mInteractableQuad.h,
                }, 0.01f, optionColor);

            // Drawing Option Circle
            renderer->addCircle(smath::vec2(mInteractableQuad.x + 8.0f * renderer->getUIScale(), optionYOffset + mInteractableQuad.h * 0.5f), 3.0f * renderer->getUIScale(), 0.03f, Color(1), 18);

            // Drawing Value
            renderer->addText(mMaterialOptions[i].mName, smath::vec2(mInteractableQuad.x + mInteractableQuad.w, mInteractableQuad.h * 0.5f) + smath::vec2(-1.0f * renderer->getUIScale(), optionYOffset), 0.02f, renderer->getTextSize(TEXT_NORMAL), Color(1.0f), TEXT_H_RIGHT, TEXT_V_MIDDLE);
        }

        // Reverting to old layer 
        renderer->setActiveLayer(oldLayer);
    }
}

// Definitioon for private function for generating material name
void MaterialDropdown::storeMaterialName()
{
    // Checking for the game object's material name
    if (!mpObject)
    {
        mStoredMaterialName = "INVALID GAMEOBJECT";
        return;
    }

    // Attempting to access the material manager
    MaterialManager* manager = ResourceManager::getInstance()->getMaterialManager();
    if (!manager)
    {
        mStoredMaterialName = "NO MATERIAL MANAGER";
        return;
    }

    // Attempting to get the specific material
    Material* material = manager->getMaterial(mpObject->getMaterialKey());
    if (!material)
    {
        mStoredMaterialName = "DEFAULT MATERIAL";
        return;
    }

    // Outputting the material name (finally)
    mStoredMaterialName = material->getName();
}