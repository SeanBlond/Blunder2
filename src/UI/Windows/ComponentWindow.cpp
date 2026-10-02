#include "ComponentWindow.h"

// Element related functions
void ComponentWindow::removeAllComponentGroups()
{
    // Looping through each component group and deallocating it
    for (ComponentUIGroup* compGroup : mpComponentGroups)
    {
        delete compGroup;
        compGroup = nullptr;
    }
    mpComponentGroups.clear();
}
void ComponentWindow::addComponentGroup(ComponentUIGroup* componentGroup)
{
	mpComponentGroups.push_back(componentGroup);
}
void ComponentWindow::generateComponentGroups()
{
	// Deleting old components
	removeAllComponentGroups();

	// Checking if the referenced GameObject is valid
	if (!mpGameObject)
		return;

	// Making Renderer & UI scale more accessible
	GraphicsRenderer* renderer = GraphicsDevice::getInstance()->getRenderer();
	float uiScale = renderer->getUIScale();

	// Determining the starting y position of the component groups
	float yPos = 40 * uiScale + mPosition.getYOffset();


	// * --------------------------- *
	// |  Transform Component Group  |
	// * --------------------------- *
	
	// Generating component group for the transform and increasing yPos
	ComponentUIGroup* transformGroup = new ComponentUIGroup("Transform");
	yPos += transformGroup->mQuad.h + uiScale * 5;

	// Adding float entry for x-axis
	float* xAxis = &(mpGameObject->getTransform()->position.x);
	transformGroup->mpElements.push_back(new FloatEntry(UIQuad(), "X",  xAxis));
	yPos += transformGroup->mpElements.back()->getQuad().h + uiScale * 5;

	// Adding float entry for y-axis
	float* yAxis = &(mpGameObject->getTransform()->position.y);
	transformGroup->mpElements.push_back(new FloatEntry(UIQuad(), "Y", yAxis, -10.0f, 10.0f));
	yPos += transformGroup->mpElements.back()->getQuad().h + uiScale * 5;

	// Adding the component group to the vector
	addComponentGroup(transformGroup);
}
void ComponentWindow::updateComponentGroupPositions()
{
	// If there are no groups, do nothing
	if (mpComponentGroups.size() <= 0)
		return;

	// Making UI scale more accessible
	float uiScale = GraphicsDevice::getInstance()->getRenderer()->getUIScale();

	// Determining the starting y position of the component groups
	float yPos = 40 * uiScale + mPosition.getYOffset();

	// Looping through each component group and updating its position
	for (ComponentUIGroup* compGroup : mpComponentGroups)
	{
		compGroup->mQuad = UIQuad(
			smath::vec2(mPosition.getXOffset() + 5 * uiScale, yPos),
			smath::vec2(mPosition.getWidth() - 10 * uiScale, 40 * uiScale)
		);
		yPos += compGroup->mQuad.h + uiScale * 5;

		// If the group is dropped down, update the position of all its elements
		if (compGroup->mDroppedDown)
		{
			for (UIElement* element : compGroup->mpElements)
			{
				// Setting position
				element->setQuad(UIQuad(
						smath::vec2(mPosition.getXOffset() + 10 * uiScale, yPos),
						smath::vec2(mPosition.getWidth() - 20 * uiScale, 40 * uiScale)
				));

				// Updating interactable
				element->generateInteractable();

				// Updating yPos
				yPos += element->getQuad().h + uiScale * 5;
			}
		}
	}
}
UIElement* ComponentWindow::getElementAtPos(smath::ivec2 position)
{
	// Looping through each element in each component group to check for collisions
	for (ComponentUIGroup* compGroup : mpComponentGroups)
	{
		// If the component group isn't dropped down, continue
		if (!(compGroup->mDroppedDown))
			continue;

		for (UIElement* element : compGroup->mpElements)
		{
			if (element->checkCollision(position))
			{
				return element;
			}
		}
	}

	// Looping through each element to check for collisions
	for (UIElement* element : mpElements)
	{
		if (element->checkCollision(position))
		{
			return element;
		}
	}

	// No collision detected, return null
	return nullptr;
}

// Override functions (UIWindow)
void ComponentWindow::OpenWindow()
{
	// Generating component groups
	generateComponentGroups();

	// Updating the positions of all the component groups
	updateComponentGroupPositions();
}
void ComponentWindow::CloseWindow()
{
	// Does nothing yet
}
void ComponentWindow::ResizeWindow()
{
	// Updating position of component groups
	updateComponentGroupPositions();
}
void ComponentWindow::DrawWindow(GraphicsRenderer* renderer)
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

	// Keeping track of yPos for compGroups
	float uiScale = renderer->getUIScale();
	float yPos = 40 * uiScale + mPosition.getYOffset();

	// Looping through each component group and drawing it
	for (ComponentUIGroup* compGroup : mpComponentGroups)
	{
		// Drawing header rectangle for the group
		renderer->addRectangle(compGroup->mQuad.getSDL_FRect(), 0.02f, Color(0.3f));
		renderer->addText(compGroup->mName, compGroup->mQuad.Center(), 0.03f, renderer->getTextSize(TEXT_LARGE), Color(1), TEXT_H_CENTER, TEXT_V_MIDDLE);


		// If the group is dropped down, draw the elements
		if (compGroup->mDroppedDown)
		{
			// Drawing a rectangle around all the elements
			float borderHeight = compGroup->mpElements.size() * (45 * uiScale) + (5 * uiScale);
			renderer->addRectangle(
				compGroup->mQuad.BottomLeft(),
				smath::vec2(compGroup->mQuad.w, borderHeight),
				0.03f, Color(0.2f)
			);

			for (UIElement* element : compGroup->mpElements)
			{
				element->drawElement(renderer, 0.04f);
			}
		}
	}
}