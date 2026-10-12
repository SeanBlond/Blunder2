#include "HierarchyTextEntry.h"
#include "../../Runtime/Time.h"

// Override function definitions
void HierarchyTextEntry::generateInteractable()
{
    // Deleting old interactable
    delete mpInteractable;

    // Generating new one
    mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}
void HierarchyTextEntry::onClick(MouseClickData clickData)
{
    // Checking if the click time is within the time bound of the last click
    mDoubleClicked = (Time::getTimeInSeconds() - mFirstClickTime <= msDoubleClickLimit);

    // Storing new time
    mFirstClickTime = Time::getTimeInSeconds();
}
void HierarchyTextEntry::onRelease(MouseClickData clickData)
{
    // Checking what to do depending on click timing
    if (!mDoubleClicked)
    {
        // Single click, set active object
        EventSystem::getInstance()->fire(ActiveObjectChangeEvent(mpRefObject));
    }
    else
    {
        // Double click, Starting type
        startTyping(*mpValue);
        selectAll();
    }
}
void HierarchyTextEntry::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
    // Drawing a rect around the text entry while typing
    if (getTyping())
        renderer->addRectangle(mQuad.getSDL_FRect(), layerOffset, Color(0.1f));

    // Drawing the text
    drawTextInput(renderer, mQuad.MiddleLeft() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f), layerOffset + 0.02f, Color(1.0f), TEXT_H_LEFT, TEXT_V_MIDDLE);
}
void HierarchyTextEntry::handleInput()
{
    // Setting new value
    setValue(getInputText());
}