#include "Float3Entry.h"

// Deconstructor
Float3Entry::~Float3Entry()
{
    delete mpClickEvent;
    mpClickEvent = nullptr;
}

// Element function definitions
uint8_t Float3Entry::determineAxisSelected(const smath::vec2& position)
{
    // Looping through each of the axis x positions and returning which is closest
    uint8_t closestAxis = 0;
    float closestAxisDistance = abs(position.x - mAxisXPositions[0]);
    for (int i = 1; i < 3; i++)
    {
        if (abs(position.x - mAxisXPositions[i]) < closestAxisDistance)
            closestAxis = i;
    }

    // Returning the closest axis
    return closestAxis;
}
float* Float3Entry::getSelectedAxis()
{
    // Returning different float pointers depending on the axis
    float* outputFloat = &(mpValue->x);
    switch (mAxisSelected)
    {
    case 0:
        break;
    case 1:
        outputFloat = &(mpValue->y);
        break;
    case 2:
        outputFloat = &(mpValue->z);
        break;
    }

    // Returning the float value
    return outputFloat;
}

// Override function definitions
void Float3Entry::generateInteractable()
{
    // Deleting old interactable
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

    // Storing the x positions of the different axis texts
    float startingXPos = mInteractableQuad.x + (mInteractableQuad.w / 3.0f * 0.5f);
    for (int i = 0; i < 3; i++)
    {
        mAxisXPositions[i] = startingXPos + (i * mInteractableQuad.w / 3.0f);
    }
}
void Float3Entry::onClick(MouseClickData clickData)
{
    // Storing initial mouse pos and saving value
    mSlideStarted = false;
    mSaveValue = *getSelectedAxis();
    mInitialMousePos = clickData.mPosition;

    // Calculating which axis is being selected
    mAxisSelected = determineAxisSelected(clickData.mPosition);
}
void Float3Entry::onHold(MouseClickData clickData)
{
    // Checking if value should slide with mouse
    if (abs(clickData.mPosition.x - mInitialMousePos.x) > 10.0f || mSlideStarted)
    {
        mSlideStarted = true;
        float newValue = mSaveValue + (clickData.mPosition.x - mInitialMousePos.x) * mSpeed;
        *getSelectedAxis() = newValue;
    }
}
void Float3Entry::onRelease(MouseClickData clickData)
{
    // Checking if text input should be activated
    if (!mSlideStarted)
    {
        startTyping(smath::round(*getSelectedAxis(), 3));
        selectAll();
    }
    else
    {
        mSlideStarted = false;
    }
}
void Float3Entry::drawElement(GraphicsRenderer* renderer, float layerOffset)
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
        smath::vec2(mInteractableQuad.x - renderer->getUIScale() * 4, mQuad.getCenterY()), layerOffset + 0.01f, renderer->getTextSize(TEXT_NORMAL),
        Color(1), TEXT_H_RIGHT, TEXT_V_MIDDLE);

    // Drawing the text for each axis
    for (int i = 0; i < 3; i++)
    {
        Color rectColor = ((!getTyping() && !mClicked) || i == mAxisSelected) ? Color(0.25f) * effect : Color(0.25f);

        // Drawing rects for each of the axis texts
        renderer->addCenteredRectangle(
            smath::vec2(mAxisXPositions[i], mQuad.getCenterY()), 
            smath::vec2(mInteractableQuad.w * 0.333f - renderer->getUIScale() * 4.0f, mQuad.h), 
            layerOffset, rectColor);
        
        if (getTyping() && i == mAxisSelected)
        {
            drawTextInput(renderer, smath::vec2(mAxisXPositions[i], mQuad.getCenterY() - renderer->getUIScale() * 3.0f), layerOffset + 0.02f, Color(1.0f), TEXT_H_CENTER, TEXT_V_MIDDLE);
        }
        else
        {
            float axisValue = *(&(mpValue->x) + i);
            renderer->addText(
                smath::round(axisValue, 3),
                smath::vec2(mAxisXPositions[i], mQuad.getCenterY() - renderer->getUIScale() * 3.0f), layerOffset + 0.02f,
                renderer->getTextSize(TEXT_NORMAL), Color(1.0f), TEXT_H_CENTER, TEXT_V_MIDDLE);
        }
    }
}
void Float3Entry::handleInput()
{
    // Try to cast text to be a string
    float tempValue = *getSelectedAxis();
    try
    {
        tempValue = std::stof(getInputText());
    }
    catch (std::invalid_argument)
    {
        std::cout << "ERROR: Float Value could not be assigned from \"" << getInputText() << "\"" << std::endl;
    }

    // Setting the value
    *getSelectedAxis() = tempValue;
}