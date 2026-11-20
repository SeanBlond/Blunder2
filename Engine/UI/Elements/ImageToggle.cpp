#include "ImageToggle.h"

// Override function definitions
void ImageToggle::generateInteractable()
{
	// Delete the old interactable
	delete mpInteractable;

	// Creating the new interactable
	mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}
void ImageToggle::onClick(MouseClickData clickData)
{
	// Returning if the button isn't right
	if (clickData.mButton != SDL_BUTTON_LEFT)
		return;

	// Toggling the value
	(*mValue) = !(*mValue);

	// Firing the event (if applicable)
	if (mClickEvent)
		EventSystem::getInstance()->fire(*mClickEvent);
}
void ImageToggle::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
	// Getting the color effetc
	Color effect = 1.0f;
	if (mClicked)
		effect = 0.5f;
	else if (mHighlighted)
		effect = 0.75f;

	// Drawing the image
	renderer->addTexQuad(mQuad.getSDL_FRect(), layerOffset, (*mValue ? mTrueImageKey : mFalseImageKey), effect);
}