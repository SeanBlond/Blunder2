#include "ViewportWindow.h"

// Data management function definitions
void ViewportWindow::addEvents()
{
	// Adding the necessary events
	EventSystem::getInstance()->addListener(EVENT_MOUSE_SCROLL, this);
	EventSystem::getInstance()->addListener(EVENT_SOFTWARE_VIEWPORT_RESET, this);
}
smath::vec2 ViewportWindow::getRelativePosition(smath::vec2 pos)
{
    // Getting the position relative to the view quad
    smath::vec2 relPos = (pos - mViewQuad.getPosition()) / (mViewQuad.getSize()) * 2.0f - 1.0f;

    // Converting the view quad relative position to coordinates in the game
    smath::vec2 viewScale = smath::vec2(1.0f, 1.0f / mViewQuad.getAspectRatio()) * calculateViewportScale();
    smath::vec2 worldPos = relPos * viewScale;

    // Returning the properly found position
    return worldPos;
}

// Element related functions
void ViewportWindow::updateElements()
{
    // Does nothing yet
}

// Override functions (UIWindow)
void ViewportWindow::OpenWindow()
{
    // Does nothing yet
}
void ViewportWindow::CloseWindow() 
{
    // Does nothing yet
}
void ViewportWindow::ResizeWindow() 
{
	// Making renderer easily accesible
	GraphicsRenderer* renderer = GraphicsDevice::getInstance()->getRenderer();

	// Determing display quad
    mViewQuad = UIQuad(
		renderer->getUIScale() * 5 + mPosition.getXOffset(), 
		renderer->getUIScale() * 40 + mPosition.getYOffset(),
		mPosition.getWidth() - (renderer->getUIScale() * 10), 
		mPosition.getHeight() - (renderer->getUIScale() * 45)
	);

    // Setting the size of the game objecet display
	renderer->getGameObjectBuffer()->updateTextureSize(smath::ivec2((int)mViewQuad.w, (int)mViewQuad.h));
}
void ViewportWindow::DrawWindow(GraphicsRenderer* renderer) 
{
	// Setting the viewport
	renderer->setNewViewport(mPosition.getRect());

	// Drawing the background 
	renderer->addRectangle(mPosition.getRect(), 0.0f, Color(0.25f));

	// Drawing the window title text
	renderer->addText(mWindowName, smath::vec2(5, 25) * renderer->getUIScale() + mPosition.getOffset(), 0.01f, renderer->getTextSize(TEXT_LARGE));
	renderer->addText(mViewportPosition, smath::vec2(150, 25) * renderer->getUIScale() + mPosition.getOffset(), 0.01f, renderer->getTextSize(TEXT_LARGE));

	// Drawing the line under the text
	renderer->addLine(
		smath::vec2(5, 35) * renderer->getUIScale() + mPosition.getOffset(),
		smath::vec2(mPosition.getWidth(), 0.0f) + smath::vec2(-5, 35) * renderer->getUIScale() + mPosition.getOffset(),
		0.01f, Color(0.2f), 3.0f
	);

	// Setting the draw position of the viewport
	renderer->getGameObjectBuffer()->getCamera()->mScale = calculateViewportScale();
	renderer->getGameObjectBuffer()->getCamera()->mPosition = smath::vec2(0) - mViewportPosition;
	renderer->getGameObjectBuffer()->setDisplayPosition(mViewQuad.getSDL_FRect(), 0.02f);
}

// Override functions (EventListener)
void ViewportWindow::handleEvent(const Event& event)
{
    // Ensuring the event type is correct
    if (event.getType() == EVENT_MOUSE_SCROLL)
    {
        // Casitng the event
        MouseScrollEvent castEvent = static_cast<const MouseScrollEvent&>(event);

        // Checking if the event happens within the window bounds
        if (!checkUICollision(castEvent.getPosition(), mPosition.getRect()))
            return;

        // Checking if the scroll event should be a pan or zoom
        if (std::fmodf(castEvent.getScroll().x, 1.0f) != 0.0f || std::fmodf(castEvent.getScroll().y, 1.0f) != 0.0f)
        {
            // Scroll values have fractions, thus they are qualified as panning
            mViewportPosition += castEvent.getScroll() * smath::vec2(1, -1) * (1.0f / calculateViewportScale());
        }
        else
        {
            // Zooming in on the mouse pos
            smath::vec2 mouseBeforePos = getRelativePosition(castEvent.getPosition()) + mViewportPosition;
            mViewportPosition = mouseBeforePos;
            mViewportScaleValue += castEvent.getScroll().y * mSensitivity;

            // Changing the offset so the highlighted spot stays at the same pos of the mouse
            smath::vec2 mouseAfterPos = getRelativePosition(castEvent.getPosition());
            mViewportPosition = mouseBeforePos - mouseAfterPos;
        }
    }
    else if (event.getType() == EVENT_SOFTWARE_VIEWPORT_RESET)
    {
        // Resetting the viewport
        mViewportScaleValue = 10.0f;
        mViewportPosition = smath::vec2(0);
    }
}
float ViewportWindow::calculateViewportScale()
{
    // Running the mViewportScaleValue through the function f(x) = 10^x
    float outputScale = pow(10.0f, mViewportScaleValue);
    return outputScale;
}