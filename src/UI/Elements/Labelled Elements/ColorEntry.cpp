#include "ColorEntry.h"

// Deconstructor
ColorEntry::~ColorEntry()
{
    delete mpClickEvent;
    mpClickEvent = nullptr;
}

// Override function definitions
void ColorEntry::generateInteractable()
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
void ColorEntry::onRelease(MouseClickData clickData)
{
    // TODO: Open Color Window
    std::cout << "Open Color Window Pop-Up" << std::endl;
}
void ColorEntry::drawElement(GraphicsRenderer* renderer, float layerOffset)
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

    // Drawing a rect for the color
    renderer->addRectangle(mInteractableQuad.getSDL_FRect(), layerOffset, (*mpValue) * effect);
    renderer->addTexQuad(mInteractableQuad.TopLeft(), smath::vec2(mInteractableQuad.h), layerOffset + 0.01f, "Grid_Symbol", (*mpValue));

}