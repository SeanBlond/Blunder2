#include "Text.h"

// Override functions
void Text::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
	// Drawing the text
	renderer->addText(mText, mQuad.getPosition(), layerOffset, mSize, mColor, mHorizAlign, mVertAlign);
}