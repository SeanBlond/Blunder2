#include "HierarchyWindow.h"

// HierarchyGroup & Element related function definitions (some overrides)
void HierarchyWindow::removeAllHierarchyGroups()
{
	for (HierarchyUIGroup* group : mpHierarchyGroups)
	{
		delete group;
		group = nullptr;
	}
	mpHierarchyGroups.clear();
}
void HierarchyWindow::addGroupFromObject(GameObject* object, float uiScale, float& yPos, int indent)
{
    // If the object is null, return null
    if (!object)
		return;

	// Creating a new group
	HierarchyUIGroup* group = new HierarchyUIGroup(object);
	
	// Setting the group position
	group->mQuad = UIQuad(
		mPosition.getXOffset() + ((float)indent * INDENT_SCALE * uiScale) + 5 * uiScale,
		yPos,
		mPosition.getWidth() - ((float)indent * INDENT_SCALE * uiScale + 10 * uiScale),
		uiScale * 25
	);

	// Adding offset to yPos
	yPos += group->mQuad.h + 5 * uiScale;

	// Adding text input to the group
	group->mpElements.push_back(new HierarchyTextEntry(
		UIQuad(
			group->mQuad.x + uiScale * 25,
			group->mQuad.y,
			group->mQuad.w - uiScale * 50,
			group->mQuad.h
		),
		object->getNameAddress(),
		object
	));

	// Adding visibility toggle to the group
	group->mpElements.push_back(new ImageToggle(
		UIQuad(
			group->mQuad.x + group->mQuad.w - uiScale * 25,
			group->mQuad.y,
			uiScale * 20,
			uiScale * 20
		),
		&(object->getObjectUIData()->mVisible),
		"Show_True_Symbol",
		"Show_False_Symbol"
	));

	// Adding dropdown toggle to the group (if the object has children)
	if (object->getChildrenCount() > 0)
	{
		group->mpElements.push_back(new ImageToggle(
			UIQuad(
				group->mQuad.x,
				group->mQuad.y,
				uiScale * 20,
				uiScale * 20
			),
			&(object->getObjectUIData()->mDroppedDown),
			"Dropdown_True_Symbol",
			"Dropdown_False_Symbol",
			new SoftwareRuntimeEvent(EVENT_SOFTWARE_HIERARCHY_DROPDOWN)
		));
	}

	// Adding the group to the vector
	mpHierarchyGroups.push_back(group);

	// If the object has any children, and it is dropped down, add those children
	if (object->getChildrenCount() > 0 && object->getObjectUIData()->mDroppedDown)
	{
		for (int i = 0; i < object->getChildrenCount(); i++)
		{
			addGroupFromObject(object->getChildAtIndex(i), uiScale, yPos, indent + 1);
		}
	}
}
void HierarchyWindow::generateHierarchyGroups()
{
	// Deleting all previous groups
	removeAllHierarchyGroups();

	// Checking if the root is null
	if (!mpRootObject)
		return;

	// Defining starting values
	float uiScale = GraphicsDevice::getInstance()->getRenderer()->getUIScale();
	float yPos = mPosition.getYOffset() + 45 * uiScale;

	// Looping through all of the root objects children and creating gruops from them
	for (int i = 0; i < mpRootObject->getChildrenCount(); i++)
	{
		addGroupFromObject(mpRootObject->getChildAtIndex(i), uiScale, yPos, 0);
	}
}
void HierarchyWindow::updateHierarchyGroupPositions()
{

}
UIElement* HierarchyWindow::getElementAtPos(smath::ivec2 position) 
{
	// Looping through each element in each hierarchy group to check for collisions
	for (HierarchyUIGroup* hierGroup : mpHierarchyGroups)
	{
		// If the hierarchy group isn't dropped down, check dropdwon button then continue
		if (!(hierGroup->getDroppedDown()))
		{
			// Checking dropdown element
			UIElement* dropdown = hierGroup->mpElements.back();
			if (dropdown->checkCollision(position))
				return dropdown;
		}

		for (UIElement* element : hierGroup->mpElements)
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

// Override function definitions (UIWindow)
void HierarchyWindow::OpenWindow() 
{
	// Adding event listener
	EventSystem::getInstance()->addListener(EVENT_SOFTWARE_HIERARCHY_DROPDOWN, this);

	// Generating the groups
	generateHierarchyGroups();
}
void HierarchyWindow::CloseWindow() 
{

}
void HierarchyWindow::ResizeWindow() 
{
	// Regenerating the groups
	generateHierarchyGroups();
}
void HierarchyWindow::DrawWindow(GraphicsRenderer* renderer) 
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

	// Drawing all the hieerarchy groups
	for (HierarchyUIGroup* group : mpHierarchyGroups)
	{
		// Checking if the group's object is the active object
		if (group->mpObject == mpActiveObject)
		{
			// Drawing a light rectangle around the object
			renderer->addRectangle(group->mQuad.getSDL_FRect(), 0.02f, Color(0.25f));
		}

		// Drawing all the elements within the group
		for (UIElement* element : group->mpElements)
		{
			element->drawElement(renderer, 0.03f);
		}
	}
}

// Override function definitions (EventListener)
void HierarchyWindow::handleEvent(const Event& event) 
{
	// Ensuring the propper event was received
	if (event.getType() == EVENT_SOFTWARE_HIERARCHY_DROPDOWN)
	{
		// Regenerating hierarchy groups to account for dropdown change
		generateHierarchyGroups();
	}
}