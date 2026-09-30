#include "Dropdown.h"

// Override functions
void Dropdown::generateInteractable()
{
    // Delete the old interactable
    delete mpInteractable;

    // Creating the new interactable
    mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}

void Dropdown::onClick(MouseClickData clickData)
{
    // Activating dropdown
    mDroppedDown = true;

    // Clamping value to bhe within the options range
    *mpIndex = smath::clamp(*mpIndex, 0, (int)mOptions.size());
}
void Dropdown::onHold(MouseClickData clickData)
{
    float lowestPoint = mQuad.y + mQuad.h + (mQuad.h * mOptions.size());
    bool directionDown = (lowestPoint < GraphicsDevice::getInstance()->getDisplayHeight());

    // Checking each options for a mouse y-collision
    for (int i = 0; i < mOptions.size(); i++)
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
            *mpIndex = i;
            break;
        }
    }
}
void Dropdown::onRelease(MouseClickData clickData)
{
    // Deactivating dropdown
    mDroppedDown = false;
}
void Dropdown::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
    // Getting the color effetc
    Color effect = 1.0f;
    if (mClicked)
        effect = 0.5f;
    else if (mHighlighted)
        effect = 0.75f;

    // Drawing Dropdown Box
    renderer->addRectangle(mQuad.getSDL_FRect(), layerOffset, Color(0.2f) * effect);
    
    // Drawing text for the current value
    renderer->addText(mOptions[*mpIndex], mQuad.MiddleRight() - smath::vec2(1.0f * renderer->getUIScale(), 0.0f), layerOffset + 0.01f, renderer->getTextSize(TEXT_NORMAL), Color(1.0f), TEXT_H_RIGHT, TEXT_V_MIDDLE);

    // Drawing Dropdown Icon
    std::string dropdownKey = (mDroppedDown ? "Dropdown_True_Symbol" : "Dropdown_False_Symbol");
    renderer->addTexQuad(mQuad.MiddleLeft() + smath::vec2(8.0f * renderer->getUIScale(), 0.0f), smath::vec2(mQuad.h * 0.5f), 0.04f, dropdownKey, Color(1), true);

    // Drawing each option if dropped down
    if (mDroppedDown)
    {
        // Storing the old renderer layer 
        RenderLayer oldLayer = renderer->getCurrentLayer();

        // Setting new layer 
        renderer->setActiveLayer(UI_POPUP_LAYER);

        // Setting optionYSize for UI interaction
        float optionWidth = mQuad.w;

        // Checking if there is enough space for the dropdown to drop downwards
        float lowestPoint = mQuad.y + mQuad.h + (mQuad.h * mOptions.size());
        bool directionDown = (lowestPoint < GraphicsDevice::getInstance()->getDisplayHeight());

        // Drawing a quad that outlines the options
        SDL_FRect outlineCorner =
        {
            mQuad.x - (renderer->getUIScale() * 5.0f),
            mQuad.y + (directionDown ? mQuad.h : -(mQuad.h * mOptions.size()) - (renderer->getUIScale() * 5.0f)),
            mQuad.w + (renderer->getUIScale() * 10.0f),
            (mQuad.h * mOptions.size()) + (renderer->getUIScale() * 5.0f)
        };
        renderer->addRectangle(outlineCorner, 0.0f, Color(0.25f));

        // Drawing each option
        for (int i = 0; i < mOptions.size(); i++)
        {
            float optionYOffset = mQuad.y + (directionDown ? 1 : -1) * (mQuad.h * (i + 1));

            // Highlighting the option if it is currently selected
            Color optionColor = (i % 2 == 0 ? Color(0.225f) : Color(0.2f));
            optionColor = (i == *mpIndex ? Color(0.275f) : optionColor);

            // Drawing Dropdown Box
            renderer->addRectangle(
                {
                    mQuad.x,
                    optionYOffset,
                    mQuad.w,
                    mQuad.h,
                }, 0.01f, optionColor);

            // Drawing Option Circle
            renderer->addCircle(smath::vec2(mQuad.x + 8.0f * renderer->getUIScale(), optionYOffset + mQuad.h * 0.5f), 3.0f * renderer->getUIScale(), 0.03f, Color(1), 18);

            // Drawing Value
            renderer->addText(mOptions[i], smath::vec2(mQuad.x + mQuad.w, mQuad.h * 0.5f) + smath::vec2(-1.0f * renderer->getUIScale(), optionYOffset), 0.02f, renderer->getTextSize(TEXT_NORMAL), Color(1.0f), TEXT_H_RIGHT, TEXT_V_MIDDLE);
        }

        // Reverting to old layer 
        renderer->setActiveLayer(oldLayer);
    }
}