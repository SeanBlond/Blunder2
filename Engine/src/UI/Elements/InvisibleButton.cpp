#include "InvisibleButton.h"

// Override function definitions
void InvisibleButton::generateInteractable()
{
	// Delete the old interactable
	delete mpInteractable;

	// Creating the new interactable
	mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}
void InvisibleButton::onClick(MouseClickData clickData)
{
	// Returning if the button isn't right
	if (clickData.mButton != SDL_BUTTON_LEFT)
		return;

	// Firing the event (if applicable)
	if (mClickEvent)
		EventSystem::getInstance()->fire(*mClickEvent);
}
void InvisibleButton::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
	// Does nothing
}