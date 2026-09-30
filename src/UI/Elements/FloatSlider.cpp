#include "FloatSlider.h"

// Override Functions Definitions
void FloatSlider::generateInteractable()
{
    // Deleting old interactable
    delete mpInteractable;

    // Creating new interactable
    mpInteractable = new QuadInteractable(mQuad.getSDL_FRect());
}
void FloatSlider::onClick(MouseClickData clickData)
{
    // Returning if the button isn't right
    if (clickData.mButton != SDL_BUTTON_LEFT)
        return;

    // Converting click position to value
    setValueByLerpedPos((float)clickData.mPosition.x);

    // If there is a click event, fire it
    if (mpClickEvent)
        EventSystem::getInstance()->fire(*mpClickEvent);
}
void FloatSlider::onHold(MouseClickData clickData)
{
    // Returning if the button isn't right
    if (clickData.mButton != SDL_BUTTON_LEFT)
        return;

    // Converting hold position to value
    setValueByLerpedPos((float)clickData.mPosition.x);

    // If there is a click event, fire it
    if (mpClickEvent)
        EventSystem::getInstance()->fire(*mpClickEvent);
}
void FloatSlider::drawElement(GraphicsRenderer* renderer, float layerOffset)
{
    Color lineColor = Color(0.7f);
    if (mClicked)
        lineColor = Color(1.0f);
    else if (mHighlighted)
        lineColor = Color(0.85f);

    // Drawing the base lines
    renderer->addLine(
        smath::vec2(mQuad.BottomLeft()),
        smath::vec2(mQuad.TopLeft()),
        layerOffset, lineColor,
        3.0f * renderer->getUIScale()
    );
    renderer->addLine(
        smath::vec2(mQuad.BottomRight()),
        smath::vec2(mQuad.TopRight()),
        layerOffset, lineColor,
        3.0f * renderer->getUIScale()
    );
    renderer->addLine(
        smath::vec2(mQuad.MiddleLeft()),
        smath::vec2(mQuad.MiddleRight()),
        layerOffset, lineColor,
        3.0f * renderer->getUIScale()
    );

    // Drawing the circle at the default value of the slider (if applicable)
    if (mDefaultValueSet)
    {
        renderer->addCircle(
            smath::vec2(getXPosFromValue(mDefaultValue), mQuad.getCenterY()),
            5.0f * renderer->getUIScale(),
            layerOffset, lineColor, 16
        );
    }

    // Drawing the "blank" rectangle inside the slider
    renderer->addRectangle(
        smath::vec2(getXPosFromPercentage(getPercentage()), mQuad.MiddleLeft().y),
        smath::vec2(6.0f, 12.5) * renderer->getUIScale(),
        layerOffset + 0.01f, Color(0.25f)
    );
    // Drawing the slider as an outline
    renderer->addRectangleOutline(
        smath::vec2(getXPosFromPercentage(getPercentage()), mQuad.MiddleLeft().y),
        smath::vec2(6.0f, 12.5) * renderer->getUIScale(),
        layerOffset + 0.02f, lineColor,
        3.0f * renderer->getUIScale()
    );
}