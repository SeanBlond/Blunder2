#include "TestWindow.h"

// Element related functions
void TestWindow::updateElements()
{
    // Does nothing yet
}

// Override functions (UIWindow)
void TestWindow::OpenWindow()
{
    // Does nothing yet
}
void TestWindow::CloseWindow() 
{
    // Does nothing yet
}
void TestWindow::ResizeWindow() 
{
    // Does nothing yet
}
void TestWindow::DrawWindow(GraphicsRenderer* renderer) 
{
	// Setting the viewport
	renderer->setNewViewport(mPosition.getRect());

	// Drawing the background 
	renderer->addRectangle(mPosition.getRect(), 0.0f, Color(0.25f));

	// Drawing the window title text
	renderer->addText(mWindowName, smath::vec2(5, 25) * renderer->getUIScale() + mPosition.getOffset(), 0.01f, renderer->getTextSize(TEXT_LARGE));

	// Drawing the line under the text
	renderer->addLine(
		smath::vec2(5, 35) * renderer->getUIScale() + mPosition.getOffset(),
		smath::vec2(mPosition.getWidth(), 0.0f) + smath::vec2(-5, 35) * renderer->getUIScale() + mPosition.getOffset(),
		0.01f, Color(0.2f), 3.0f
	);

	// Drawing a background to the histogram
	renderer->addRectangle(
		{
			renderer->getUIScale() * 5 + mPosition.getXOffset(), renderer->getUIScale() * 40 + mPosition.getYOffset(),
			mPosition.getWidth() - (renderer->getUIScale() * 10), mPosition.getHeight() - (renderer->getUIScale() * 45)
		},
		0.01f, Color(0.2f)
	);
}