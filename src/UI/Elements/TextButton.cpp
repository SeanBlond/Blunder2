#include "TextButton.h"

// Override function definitions
void TextButton::generateInteractable()
{
	// Delete the old interactable
	delete mpInteractable;

	// Creating the new interactable
	mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}
void TextButton::onRelease(MouseClickData clickData)
{
	// Returning if the button isn't right
	if (clickData.mButton != SDL_BUTTON_LEFT)
		return;

	// Firing the event (if applicable)
	if (mClickEvent)
		EventSystem::getInstance()->fire(*mClickEvent);
}
void TextButton::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
	// Getting the color effetc
	Color effect = 1.0f;
	if (mClicked)
		effect = 0.5f;
	else if (mHighlighted)
		effect = 0.75f;

	// Drawing a rect and rect outline
	renderer->addRectangle(mQuad.getSDL_FRect(), layerOffset, Color(0.25f) * effect);
	renderer->addRectangleOutline(mQuad.getSDL_FRect(), layerOffset + 0.01f, Color(0.2f) * effect, 3.0f);

	// Drawing the text
	renderer->addText(mLabel, mQuad.Center() - smath::vec2(0.0f, renderer->getUIScale() * 3.0f), layerOffset + 0.01f, mLabelSize, mLabelColor * effect, TEXT_H_CENTER, TEXT_V_MIDDLE);
}