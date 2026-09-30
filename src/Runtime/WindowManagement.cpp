#include "WindowManagement.h"
#include "Blunder.h"

// * -------------------------- *
// |  Window Manager Functions  |
// * -------------------------- *

// Management Functions Definitions
bool WindowManager::init(const unsigned int& width, const unsigned int& height)
{
	// Defining screen size
	mScreenDimensions = smath::vec2((int)width, (int)height);

	// Creating the default windows
	mpTestWindow = new TestWindow();

	// Creating locked windows from the default windows
	mpRootWindow = new LockedWindow(mpTestWindow, nullptr, mScreenDimensions, LockedWindow::POS_NONE, GraphicsDevice::getInstance()->getRenderer()->getUIScale() * 3.0f);
	mCurrentRootWindow = WINDOW_PROJECT_LOADER;
	mpRootWindow->getWindow()->OpenWindow();
	mpRootWindow->UpdateDimensions();

	// Add Event Listseners
	EventSystem::getInstance()->addListener(EVENT_MOUSE_MOVE, this);
	EventSystem::getInstance()->addListener(EVENT_MOUSE_DOWN, this);
	EventSystem::getInstance()->addListener(EVENT_MOUSE_UP, this);
	EventSystem::getInstance()->addListener(EVENT_SOFTWARE_RESET_INTERACTION_ELEMENTS, this);

	return true;
}
void WindowManager::cleanup()
{
	delete mpRootWindow;
	mpRootWindow = nullptr;
}

// Runtime Functions Definitions
void WindowManager::updateWindows(smath::vec2 screenDimensions)
{
	// Updating the value
	mScreenDimensions = screenDimensions;

	// Updating the window sizes
	mpRootWindow->setDimensions(screenDimensions);
}
bool WindowManager::isPersistentElement(UIElement* element)
{
	switch (element->getType())
	{
	case UI_TEXT_ENTRY:
		return true;

	default:
		return false;
	}
}
void WindowManager::drawWindows(GraphicsRenderer* renderer)
{
	mpRootWindow->DrawWindows(renderer);
}
UIElement* WindowManager::checkForElementCollision(smath::vec2 pos)
{
	// Checking what window (if any) is under the mouse
	UIWindow* selectedWindow = mpRootWindow->checkForCollisions(pos);

	// If window is null, return null
	if (!selectedWindow)
		return nullptr;

	// Check for UI Element collision if window is not null
	return selectedWindow->getElementAtPos({ (int)pos.x, (int)pos.y });
}
void WindowManager::changeRootWindow(RootWindowOptions option)
{
	// Checking if the new window is the same as the old
	if (mCurrentRootWindow == option)
		return;

	// Setting the root window, and creating the respective UI window if need be
	/*switch (option)
	{
	case WINDOW_VIEWPORT:
		if (!mpViewport) { mpViewport = new ViewportWindow(); }
		mpRootWindow->ChangeWindow(mpViewport);
		break;

	case WINDOW_PROJECT_LOADER:
		if (!mpProjectLoader) { mpProjectLoader = new ProjectLoaderWindow(Shake::getInstance()->getRecentProjectManager()); }
		mpRootWindow->ChangeWindow(mpProjectLoader);
		break;

	}

	// Deleteing and setting the old root window pointer to null
	switch (mCurrentRootWindow)
	{
	case WINDOW_VIEWPORT:
		mpViewport = nullptr;
		break;

	case WINDOW_PROJECT_LOADER:
		mpProjectLoader = nullptr;
		break;
	}

	// Updating the current root window tracker
	mCurrentRootWindow = option;*/
}

// Event-related functions
void WindowManager::handleEvent(const Event& event)
{
	// Doing different things depending on the event type
	if (event.getType() == EVENT_SOFTWARE_RESET_INTERACTION_ELEMENTS)
	{
		// If this event is received, the highlighted/clicked/persistent element has been deleted and must be reset here
		mHighlightedElement = mClickedElement = mPersistentElement = nullptr;
	}
	if (event.getType() == EVENT_MOUSE_MOVE)
	{
		// Casting the event
		MouseMoveEvent castEvent = static_cast<const MouseMoveEvent&>(event);

		// Updating previously highlighted element first
		if (mHighlightedElement)
		{
			if (mHighlightedElement->checkCollision(castEvent.getPosition()))
			{
				// Collision detected! Continue highlight
				mHighlightedElement->setHighlighted(true);
			}
			else
			{
				// Unhighlighting the previously highlighted event
				mHighlightedElement->setHighlighted(false);
				mHighlightedElement = nullptr;
			}
		}

		// Trying to find a selected element
		UIElement* selectedElement = checkForElementCollision(castEvent.getPosition());

		// Deselecting the old highlighted element (if applicable)
		if (mHighlightedElement && selectedElement != mHighlightedElement)
			mHighlightedElement->setHighlighted(false);

		// Highlighting the selected element (if it exists)
		if (selectedElement)
		{

			mHighlightedElement = selectedElement;
			selectedElement->setHighlighted(true);
		}

		// Checking if there is an element currently being clicked
		if (mClickedElement)
		{
			mClickedElement->onHold(castEvent.getClickData());
		}
	}
	
	else if (event.getType() == EVENT_MOUSE_DOWN)
	{
		// Casting the event
		MouseDownEvent castEvent = static_cast<const MouseDownEvent&>(event);

		// Checking for a collided element
		UIElement* selectedElement = checkForElementCollision(castEvent.getPosition());
		if (selectedElement)
		{
			// Collision detected!
			mClickedElement = selectedElement;
			selectedElement->setClicked(true);
			selectedElement->onClick(castEvent.getClickData());
		}

		// Closing persistent element (if applicable)
		if (mPersistentElement && mPersistentElement != mClickedElement)
		{
			mPersistentElement->closeElement();
			mPersistentElement = nullptr;
		}

		// Setting persistent element (if applicable)
		if (mClickedElement && isPersistentElement(mClickedElement))
		{
			mPersistentElement = mClickedElement;
		}
	}

	else if (event.getType() == EVENT_MOUSE_UP)
	{
		// Casting the event
		MouseUpEvent castEvent = static_cast<const MouseUpEvent&>(event);

		// Checking if there is an element to release
		if (mClickedElement)
		{
			mClickedElement->setClicked(false);
			mClickedElement->onRelease(castEvent.getClickData());
			mClickedElement = nullptr;
		}
	}
}