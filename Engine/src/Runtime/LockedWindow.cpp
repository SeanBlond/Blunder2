#include "LockedWindow.h"


// Deconstructor
LockedWindow::~LockedWindow()
{
	// Deleting Window
	delete mpWindow;
	mpWindow = nullptr;

	// Deleting Connected Windows
	delete mpLeftWindow;
	mpLeftWindow = nullptr;
	delete mpRightWindow;
	mpRightWindow = nullptr;
	delete mpTopWindow;
	mpTopWindow = nullptr;
	delete mpBottomWindow;
	mpBottomWindow = nullptr;
}

// Getters
smath::vec2 LockedWindow::getMainWindowDimensions() const
{ 
	//smath::vec2 mainSpacing = smath::vec2(
	//	(mpLeftWindow ? mSpacing : 0.0f) + (mpRightWindow ? mSpacing : 0.0f),
	//	(mpBottomWindow ? mSpacing : 0.0f) + (mpTopWindow ? mSpacing : 0.0f)
	//);
	return (smath::vec2(1) - mPercentageUsed) * mDimensions;
}
float LockedWindow::getXOffset() const
{
	float xOffset = 0.0f;

	// If there is a left window, add its width to the offset
	if (mpLeftWindow)
		xOffset += mLeftWidth * mDimensions.x;

	// If there is a parent, add it's offset to this one
	if (mpParent)
	{
		xOffset += mpParent->getXOffset();

		// If this window is a left child, subtract its width from the parent offset
		if (mChildPosition == POS_LEFT)
			xOffset -= mDimensions.x;

		// If this window is a right child, add the parent window width to the offset
		else if (mChildPosition == POS_RIGHT)
			xOffset += mpParent->getMainWindowDimensions().x;
	}

	return xOffset;
}
float LockedWindow::getYOffset() const
{
	float yOffset = 0.0f;

	// If there is a top window, add its width to the offset
	if (mpTopWindow)
		yOffset += mTopHeight * mDimensions.y;

	// If there is a parent, add it's offset to this one
	if (mpParent)
	{
		yOffset += mpParent->getYOffset();

		// If this window is a top child, subtract its width from the parent offset
		if (mChildPosition == POS_TOP)
			yOffset -= mDimensions.y;

		// If this window is a bottom child, add the parent window width to the offset
		else if (mChildPosition == POS_BOTTOM)
			yOffset += mpParent->getMainWindowDimensions().y;
	}

	return yOffset;
}

// Setters
void LockedWindow::setLeftWindow(UIWindow* window, float width)
{
	// Creating the LockedWindow encapsulating the UI Window
	this->mpLeftWindow = new LockedWindow(window, this, smath::vec2(0), POS_LEFT, mSpacing);
	mpLeftWindow->setParent(this);
	setLeftWidth(width);

	// Opening the window
	mpLeftWindow->getWindow()->OpenWindow();
}
void LockedWindow::setRightWindow(UIWindow* window, float width)
{
	// Creating the LockedWindow encapsulating the UI Window
	this->mpRightWindow = new LockedWindow(window, this, smath::vec2(0), POS_RIGHT, mSpacing);
	mpRightWindow->setParent(this);
	setRightWidth(width);

	// Opening the window
	mpRightWindow->getWindow()->OpenWindow();
}
void LockedWindow::setTopWindow(UIWindow* window, float height)
{
	// Creating the LockedWindow encapsulating the UI Window
	this->mpTopWindow = new LockedWindow(window, this, smath::vec2(0), POS_TOP, mSpacing);
	mpTopWindow->setParent(this);
	setTopHeight(height);

	// Opening the window
	mpTopWindow->getWindow()->OpenWindow();
}
void LockedWindow::setBottomWindow(UIWindow* window, float height)
{
	// Creating the LockedWindow encapsulating the UI Window
	this->mpBottomWindow = new LockedWindow(window, this, smath::vec2(0), POS_BOTTOM, mSpacing);
	mpBottomWindow->setParent(this);
	setBottomHeight(height);

	// Opening the window
	mpBottomWindow->getWindow()->OpenWindow();
}

// Functions
void LockedWindow::UpdateDimensions()
{
	// Calcuting percantage used
	smath::vec2 tempPercentageUsed = smath::vec2(0);
	// X
	if (mpLeftWindow)
		tempPercentageUsed.x += mLeftWidth;
	if (mpRightWindow)
		tempPercentageUsed.x += mRightWidth;
	// Y
	if (mpTopWindow)
		tempPercentageUsed.y += mTopHeight;
	if (mpBottomWindow)
		tempPercentageUsed.y += mBottomHeight;

	mPercentageUsed = tempPercentageUsed;

	// Updating window sizes
	float topBottomWindowWidth = mDimensions.x * (1.0f - mPercentageUsed.x);
	if (mpLeftWindow)
		mpLeftWindow->setDimensions(smath::vec2(mLeftWidth * mDimensions.x, mDimensions.y), mSpacing);
	if (mpRightWindow)
		mpRightWindow->setDimensions(smath::vec2(mRightWidth * mDimensions.x, mDimensions.y), mSpacing);
	if (mpTopWindow)
		mpTopWindow->setDimensions(smath::vec2(topBottomWindowWidth, mTopHeight * mDimensions.y), mSpacing);
	if (mpBottomWindow)
		mpBottomWindow->setDimensions(smath::vec2(topBottomWindowWidth, mBottomHeight * mDimensions.y), mSpacing);

	// Resizing UI Window dimensions
	smath::vec2 mainWindowDimensions = getMainWindowDimensions();
	smath::vec2 windowOffset = getOffset();

	// Adding spacing (if applicable)
	mainWindowDimensions -= smath::vec2(
		(mpLeftWindow ? mSpacing : 0.0f) + (mpRightWindow ? mSpacing : 0.0f),
		(mpTopWindow ? mSpacing : 0.0f) + (mpBottomWindow ? mSpacing : 0.0f)
	);
	windowOffset += smath::vec2(
		(mpLeftWindow ? mSpacing : 0.0f),
		(mpTopWindow ? mSpacing : 0.0f)
	);

	mpWindow->mPosition.setPosition(mainWindowDimensions, windowOffset);
	mpWindow->ResizeWindow();
}
UIWindow* LockedWindow::checkForCollisions(smath::vec2 position)
{
	// Checking the main screen
	if (checkUICollision(position, getRect().getSDL_FRect()))
	{
		return this->mpWindow;
	}

	// Checking connected windows
	if (mpLeftWindow)
	{
		UIWindow* checkedWindow = mpLeftWindow->checkForCollisions(position);
		if (checkedWindow != nullptr)
			return checkedWindow;
	}
	if (mpRightWindow)
	{
		UIWindow* checkedWindow = mpRightWindow->checkForCollisions(position);
		if (checkedWindow != nullptr)
			return checkedWindow;
	}
	if (mpTopWindow)
	{
		UIWindow* checkedWindow = mpTopWindow->checkForCollisions(position);
		if (checkedWindow != nullptr)
			return checkedWindow;
	}
	if (mpBottomWindow)
	{
		UIWindow* checkedWindow = mpBottomWindow->checkForCollisions(position);
		if (checkedWindow != nullptr)
			return checkedWindow;
	}

	// No Collision Detected
	return nullptr;
}
void LockedWindow::DrawWindows(GraphicsRenderer* renderer)
{
	// Drawing the main window
	mpWindow->DrawWindow(renderer);

	// Drawing connected windows
	if (mpLeftWindow)
		mpLeftWindow->DrawWindows(renderer);
	if (mpRightWindow)
		mpRightWindow->DrawWindows(renderer);
	if (mpTopWindow)
		mpTopWindow->DrawWindows(renderer);
	if (mpBottomWindow)
		mpBottomWindow->DrawWindows(renderer);
}
void LockedWindow::ChangeWindow(UIWindow* newWindow)
{
	// If the new window is the same as the old, do nothing
	if (newWindow == mpWindow)
		return;

	// Calling the close function on the old window
	mpWindow->CloseWindow();

	// Deleting the old window
	delete mpWindow;

	// Assigning the new window
	mpWindow = newWindow;

	// Calling open function on the new window
	mpWindow->OpenWindow();

	// Updating the dimensions of the window
	UpdateDimensions();
}