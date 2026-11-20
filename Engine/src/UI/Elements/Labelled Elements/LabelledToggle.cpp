#include "LabelledToggle.h"

// Override function definitions
void LabelledToggle::generateInteractable()
{
    // Defining interactaqble quad
    mInteractableQuad = UIQuad(
        mQuad.x + mQuad.w * 0.3f,
        mQuad.y,
        mQuad.h,
        mQuad.h
    );

    // Creating new interactable
    mpInteractable = new QuadInteractable(mInteractableQuad.getSDL_FRect());
}
void LabelledToggle::onRelease(MouseClickData clickData)
{
    // Toggling the value
    (*mpValue) = !(*mpValue);
}
void LabelledToggle::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
    // Getting the color effetc
    float effect = 1.0f;
    if (mClicked)
        effect = 0.4;
    else if (mHighlighted)
        effect = 0.6f;

    // Drawing the label
    renderer->addText(
        mLabel,
        smath::vec2(mInteractableQuad.x - renderer->getUIScale() * 4, mQuad.getCenterY()), layerOffset + 0.01f, renderer->getTextSize(TEXT_NORMAL),
        Color(1), TEXT_H_RIGHT, TEXT_V_MIDDLE);

    // Drawingthe toggle image
    renderer->addTexQuad(
        mInteractableQuad.getSDL_FRect(), layerOffset + 0.01f,
        (*mpValue ? "Checkbox_True_Symbol" : "Checkbox_False_Symbol"),
        Color(1) * effect);
}