#include "FloatEntry.h"

// Override function definitions
void FloatEntry::generateInteractable()
{
    // Deleting old interactable
    delete mpInteractable;

    // Creating new interactable
    mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}
void FloatEntry::onClick(MouseClickData clickData)
{
    // Storing initial mouse pos and saving value
    mSlideStarted = false;
    mSaveValue = *mpValue;
    mInitialMousePos = clickData.mPosition;
}
void FloatEntry::onHold(MouseClickData clickData)
{
    // Checking if value should slide with mouse
    if (abs(clickData.mPosition.x - mInitialMousePos.x) > 10.0f || mSlideStarted)
    {
        mSlideStarted = true;
        float newValue = mSaveValue + (clickData.mPosition.x - mInitialMousePos.x) * mSpeed;
        setValue(newValue);
    }
}
void FloatEntry::onRelease(MouseClickData clickData)
{
    // Checking if text input should be activated
    if (!mSlideStarted)
    {
        startTyping(std::to_string(*mpValue));
        selectAll();
    }
    else
    {
        mSlideStarted = false;
    }
}
void FloatEntry::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
    // Getting the color effetc
    Color effect = 1.0f;
    if (getTyping())
        effect = 0.25f;
    else if (mClicked)
        effect = 0.5f;
    else if (mHighlighted)
        effect = 0.75f;

    // Drawing a rect and rect outline
    renderer->addRectangle(mQuad.getSDL_FRect(), layerOffset, Color(0.25f) * effect);
    renderer->addRectangleOutline(mQuad.getSDL_FRect(), layerOffset + 0.01f, Color(0.2f) * effect, 3.0f);

    // Determing what text to draw
    std::string drawText = "";
    if (getTyping())
    {
        drawTextInput(renderer, mQuad.Center() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f), layerOffset + 0.01f, Color(1.0f), TEXT_H_CENTER, TEXT_V_MIDDLE);
    }
    else
    {
        renderer->addText(
            std::to_string(*mpValue),
            mQuad.Center() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f), layerOffset + 0.01f,
            renderer->getTextSize(TEXT_NORMAL), Color(1.0f), TEXT_H_CENTER, TEXT_V_MIDDLE);
    }
}
void FloatEntry::handleInput()
{
    // Try to cast text to be a string
    float tempValue = (*mpValue);
    try
    {
        tempValue = std::stof(getInputText());
    }
    catch (std::invalid_argument)
    {
        std::cout << "ERROR: Float Value could not be assigned from \"" << getInputText() << "\"" << std::endl;
    }

    // Setting value
    (*mpValue) = tempValue;
}