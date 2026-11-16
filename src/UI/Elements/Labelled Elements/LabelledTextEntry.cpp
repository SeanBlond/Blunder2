#include "LabelledTextEntry.h"
#include "../../../Runtime/Time.h"

// Override function definitions
void LabelledTextEntry::generateInteractable()
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
}
void LabelledTextEntry::onRelease(MouseClickData clickData)
{
    // Starting type
    startTyping(*mpValue);
    selectAll();
}
void LabelledTextEntry::drawElement(GraphicsRenderer* renderer, float layerOffset)
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

    // Drawing a rect around the text entry
    renderer->addRectangle(mInteractableQuad.getSDL_FRect(), layerOffset, Color(0.25f) * effect);

    // Drawing a typing text while typing
    if (getTyping())
    {
        drawTextInput(renderer, mInteractableQuad.Center() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f), layerOffset + 0.02f, Color(1.0f), TEXT_H_CENTER, TEXT_V_MIDDLE);
    }
    else
    {
        // Drawing the normal text otherwise
        renderer->addText(
            (*mpValue),
            mInteractableQuad.Center() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f),
            layerOffset + 0.02f, renderer->getTextSize(TEXT_NORMAL), Color(1.0f), TEXT_H_CENTER, TEXT_V_MIDDLE
        );
    }
}
void LabelledTextEntry::handleInput()
{
    // Setting new value
    setValue(getInputText());
}