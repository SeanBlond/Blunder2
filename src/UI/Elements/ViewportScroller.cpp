#include "ViewportScroller.h"
#include "../Windows/ViewportWindow.h"

// Functions
void ViewportScroller::generateInteractable()
{
    // Deleting old interactable
    delete mpInteractable;

    // Creating new interactable
    mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}
void ViewportScroller::onClick(MouseClickData clickData)
{
    // If middle mouse button, and mpViewport exists
    if (mpViewport && clickData.mButton == SDL_BUTTON_MIDDLE)
    {
        // Storing the initial mouse pos and offset
        mInitialPos = clickData.mPosition;
        mInitialOffset = mpViewport->getOffset();
    }
}
void ViewportScroller::onHold(MouseClickData clickData)
{
    // If middle mouse button, and mpViewport exists
    if (mpViewport && clickData.mButton == SDL_BUTTON_MIDDLE)
    {
        // Calculating mouse offset
        smath::vec2 scrollOffset = mpViewport->getRelativePosition(mInitialPos) - mpViewport->getRelativePosition(clickData.mPosition);

        // Setting the offset
        mpViewport->setOffset(mInitialOffset + scrollOffset);
    }
}