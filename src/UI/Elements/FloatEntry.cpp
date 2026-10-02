#include "FloatEntry.h"

// Element function definitions
void FloatEntry::setValue(float value)
{ 
    // If clamp doesn't exist, do normal set value
    if (!mpClamps)
        *(this->mpValue) = value; 
    else
        *(this->mpValue) = smath::clamp(value, mpClamps->mMinValue, mpClamps->mMaxValue); 
}
float FloatEntry::calculateSliderWidth(float originalWidth)
{
    // If clamp doesn't exist, return 0
    if (!mpClamps)
        return 0.0f;

    float relativeValue = ((*mpValue) - mpClamps->mMinValue) / (mpClamps->mMaxValue - mpClamps->mMinValue);
    float width = originalWidth * smath::clamp01(relativeValue);
    return width;
}

// Override function definitions
void FloatEntry::generateInteractable()
{
    // Deleting old interactable
    delete mpInteractable;

    // Defining interactaqble quad
    mInteractableQuad = UIQuad(
        mQuad.getCenterX(),
        mQuad.y,
        mQuad.w * 0.5f,
        mQuad.h
    );

    // Creating new interactable
    mpInteractable = new QuadInteractable(mInteractableQuad.getSDL_FRect());
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
    float effect = 1.0f;
    if (getTyping() || mClicked)
        effect = 0.4;
    else if (mHighlighted)
        effect = 0.6f;

    // Drawing the label
    renderer->addText(
        mLabel,
        smath::vec2(mInteractableQuad.x - renderer->getUIScale() * 10, mQuad.getCenterY()), layerOffset + 0.01f, renderer->getTextSize(TEXT_NORMAL),
        Color(1), TEXT_H_RIGHT, TEXT_V_MIDDLE);

    // Drawing a rect around the text entry
    renderer->addRectangle(mInteractableQuad.getSDL_FRect(), layerOffset, Color(0.25f) * effect);

    // If there are clamp values, draw a slider
    if (mpClamps)
    {
        renderer->addRectangle(
            mInteractableQuad.getPosition(),
            smath::vec2(calculateSliderWidth(mInteractableQuad.w), mInteractableQuad.h),
            layerOffset + 0.01f, Color(0, 0.75f, 0) * effect);
    }

    // Determing what text to draw
    std::string drawText = "";
    if (getTyping())
    {
        drawTextInput(renderer, mInteractableQuad.Center() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f), layerOffset + 0.02f, Color(1.0f), TEXT_H_CENTER, TEXT_V_MIDDLE);
    }
    else
    {
        renderer->addText(
            std::to_string(*mpValue),
            mInteractableQuad.Center() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f), layerOffset + 0.02f,
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
    setValue(tempValue);
}