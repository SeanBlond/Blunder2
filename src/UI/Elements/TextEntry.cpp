#include "TextEntry.h"

// Override function definitions
void TextEntry::generateInteractable()
{
    // Deleting old interactable
    delete mpInteractable;

    // Generating new one
    mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}
void TextEntry::onRelease(MouseClickData clickData)
{
    // Starting type
    startTyping(*mpValue);
    selectAll();
}
void TextEntry::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
    // Drawing a rect around the text entry while typing
    if (getTyping())
        renderer->addRectangle(mQuad.getSDL_FRect(), layerOffset, Color(0.1f));

    // Drawing the text
    drawTextInput(renderer, mQuad.MiddleLeft() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f), layerOffset + 0.02f, Color(1.0f), TEXT_H_LEFT, TEXT_V_MIDDLE);
}
void TextEntry::handleInput()
{
    // Setting new value
    setValue(getInputText());
}