#include "MaterialWindow.h"

// Functions for generating UI
void MaterialWindow::changeMaterial(Material* material)
{
    // Checking if the material is the same as the old one
    if (material == mpMaterial)
        return; // Do nothing!

    // Setting the new material
    mpMaterial = material;

    // Updating the UI Elements
    createMaterialElements();
}
void MaterialWindow::createMaterialElements()
{
    // Removing all the old elements
    removeAllElements();

    // Checking if the material is null
    if (!mpMaterial)
        return; // Do nothing!

    // Making Renderer & UI scale more accessible
    GraphicsRenderer* renderer = GraphicsDevice::getInstance()->getRenderer();
    float uiScale = renderer->getUIScale();

    // Determining the starting y position of the elements
    float yPos = 45 * uiScale + mPosition.getYOffset();

    // Adding a button for settings
    UIElement* settingsButton = new ImageButton(UIQuad(
        smath::vec2(mPosition.getXOffset() + mPosition.getWidth() - uiScale * 45, yPos + uiScale * 5),
        smath::vec2(uiScale * 30)), 
        "Settings_Symbol", new Event(EVENT_SOFTWARE_TOGGLE_MATERIAL_EDITING));
    addElement(settingsButton);
    yPos += uiScale * 45;

    // Looping through each uniform in the material and adding it
    for (int i = 0; i < mpMaterial->getUniformCount(); i++)
    {
        Uniform* uniform = mpMaterial->getUniformAtIndex(i);

        // If the uniform is null, skip
        if (!uniform)
            continue;

        // Otherwise, create a UI element for the uniform
        UIElement* createdElement = createElementFromUniform(uniform);

        // If the element is null, continue past it
        if (!createdElement)
            continue;

        // Otherwise, add it to the vector and set it's position!
        addElement(createdElement);
        createdElement->setQuad(UIQuad(
            smath::vec2(mPosition.getXOffset() + 10 * uiScale, yPos),
            smath::vec2(mPosition.getWidth() - 20 * uiScale, 40 * uiScale)
        ));
        yPos += createdElement->getQuad().h + uiScale * 5;
    }

    // Adding a button for recompiling the material
    UIElement* recompileButton = new TextButton(UIQuad(
        smath::vec2(mPosition.getXOffset() + 10 * uiScale, yPos),
        smath::vec2(mPosition.getWidth() - 20 * uiScale, 40 * uiScale)),
        "Recompile", new Event(EVENT_SOFTWARE_RECOMPILE_MATERIAL), renderer->getTextSize(TEXT_NORMAL)
    );
    addElement(recompileButton);
    yPos += recompileButton->getQuad().h + uiScale * 5;
}
void MaterialWindow::createMaterialSettings()
{
    // Removing all the old elements
    removeAllElements();

    // Clearing the uniform type vector
    for (int* type : mpUniformTypes)
    {
        delete type;
        type = nullptr;
    }
    mpUniformTypes.clear();

    // Checking if the material is null
    if (!mpMaterial)
        return; // Do nothing!

    // Making Renderer & UI scale more accessible
    GraphicsRenderer* renderer = GraphicsDevice::getInstance()->getRenderer();
    float uiScale = renderer->getUIScale();

    // Determining the starting y position of the elements
    float yPos = 45 * uiScale + mPosition.getYOffset();

    // Adding a button for settings
    UIElement* settingsButton = new ImageButton(UIQuad(
        smath::vec2(mPosition.getXOffset() + mPosition.getWidth() - uiScale * 45, yPos + uiScale * 5),
        smath::vec2(uiScale * 30)),
        "Settings_Symbol", new Event(EVENT_SOFTWARE_TOGGLE_MATERIAL_EDITING));
    addElement(settingsButton);

    // Adding a text entry for the name
    UIElement* nameTextEntry = new TextEntry(UIQuad(
        smath::vec2(mPosition.getXOffset() + uiScale * 10, yPos),
        smath::vec2(mPosition.getWidth() - 80 * uiScale, 40 * uiScale)),
        mpMaterial->getNameAddress()
    );
    addElement(nameTextEntry);
    yPos += nameTextEntry->getQuad().h + uiScale * 5;

    // Looping through each uniform in the material and creating settings for it
    for (int i = 0; i < mpMaterial->getUniformCount(); i++)
    {
        Uniform* uniform = mpMaterial->getUniformAtIndex(i);

        // Adding a integer value for the uniform type
        mpUniformTypes.push_back((!uniform ? 0 : new int(uniform->getType())));

        // If the uniform is null, skip
        if (!uniform)
            continue;

        // Otherwise, create settings elements for it
        createSettingsFromUniform(uniform, i, uiScale, yPos);
    }

    // Adding a button for adding a new uniform
    UIElement* addUniformButton = new TextButton(UIQuad(
        smath::vec2(mPosition.getXOffset() + 10 * uiScale, yPos),
        smath::vec2(mPosition.getWidth() - 20 * uiScale, 40 * uiScale)),
        "Add Uniform", new Event(EVENT_SOFTWARE_ADD_UNIFORM), renderer->getTextSize(TEXT_NORMAL)
    );
    addElement(addUniformButton);
    yPos += addUniformButton->getQuad().h + uiScale * 5;
}
void MaterialWindow::updateElementPositions()
{
    // Making Renderer & UI scale more accessible
    GraphicsRenderer* renderer = GraphicsDevice::getInstance()->getRenderer();
    float uiScale = renderer->getUIScale();

    // Determining the starting y position of the elements
    float yPos = 45 * uiScale + mPosition.getYOffset();

    // Looping through each element and updating its quad & interactable
    for (UIElement* element : mpElements)
    {
        element->setQuad(UIQuad(
            smath::vec2(mPosition.getXOffset() + 10 * uiScale, yPos),
            smath::vec2(mPosition.getWidth() - 20 * uiScale, 40 * uiScale)
        ));
        yPos += element->getQuad().h + uiScale * 5;
    }
}
UIElement* MaterialWindow::createElementFromUniform(Uniform* uniform)
{
    // If the uniform is null, return null
    if (!uniform)
        return nullptr;

    // Defining the UI element base
    UIElement* outputElement = nullptr;

    // Allocating & creating the element based on the type
    switch (uniform->getType())
    {
    case UNIFORM_INT:
        outputElement = new IntEntry(UIQuad(), uniform->getUniformName(), static_cast<int*>(uniform->getData()));
        break;
    case UNIFORM_FLOAT:
        outputElement = new FloatEntry(UIQuad(), uniform->getUniformName(), static_cast<float*>(uniform->getData()));
        break;
    case UNIFORM_FLOAT2:
        outputElement = new Float2Entry(UIQuad(), uniform->getUniformName(), static_cast<smath::vec2*>(uniform->getData()));
        break;
    case UNIFORM_FLOAT3:
        outputElement = new Float3Entry(UIQuad(), uniform->getUniformName(), static_cast<smath::vec3*>(uniform->getData()));
        break;
    case UNIFORM_FLOAT4:
        outputElement = new Float4Entry(UIQuad(), uniform->getUniformName(), static_cast<smath::vec4*>(uniform->getData()));
        break;
    case UNIFORM_COLOR:
        outputElement = new ColorEntry(UIQuad(), uniform->getUniformName(), static_cast<Color*>(uniform->getData()));
        break;
    }

    // Returning the (potentially) newly created elements
    return outputElement;
}
void MaterialWindow::createSettingsFromUniform(Uniform* uniform, int uniformIndex, float uiScale, float& yPos)
{
    // Calculating the width of each ui element
    float elementWidth = (mPosition.getWidth() - uiScale * (25 + 45)) * 0.5f;

    // Creating a text entry for the uniform name
    UIElement* uniformNameEntry = new UniformTextEntry(UIQuad(
        smath::vec2(mPosition.getXOffset() + uiScale * 10, yPos),
        smath::vec2(elementWidth, 40 * uiScale)),
        uniform);
    addElement(uniformNameEntry);

    // Creating a dropdown for the uniform type
    UIElement* uniformTypeDropdown = new Dropdown(UIQuad(
        smath::vec2(mPosition.getXOffset() + uiScale * 15 + elementWidth, yPos),
        smath::vec2(elementWidth, 40 * uiScale)),
        mpUniformTypes[uniformIndex], UNIFORM_OPTIONS, new IntValueEvent(EVENT_SOFTWARE_UPDATE_UNIFORM_TYPE, uniformIndex));
    addElement(uniformTypeDropdown);

    // Creating a uniform delete button
    UIElement* uniformDeleteButton = new ImageButton(UIQuad(
        smath::vec2(mPosition.getXOffset() + mPosition.getWidth() - 50 * uiScale, yPos),
        smath::vec2(40 * uiScale)),
        "Trash_Symbol", new IntValueEvent(EVENT_SOFTWARE_REMOVE_UNIFORM, uniformIndex));
    addElement(uniformDeleteButton);

    yPos += uniformNameEntry->getQuad().h + uiScale * 5;
}


// Override functions (UIWindow)
void MaterialWindow::OpenWindow()
{
    // Creating UI for the material (should there be one)
    createMaterialElements();
}
void MaterialWindow::CloseWindow()
{

}
void MaterialWindow::ResizeWindow()
{
    // Resizing the elements
    updateElementPositions();
}
void MaterialWindow::DrawWindow(GraphicsRenderer* renderer)
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

    // Drawing a background to the elements
    renderer->addRectangle(
        {
            renderer->getUIScale() * 5 + mPosition.getXOffset(), renderer->getUIScale() * 40 + mPosition.getYOffset(),
            mPosition.getWidth() - (renderer->getUIScale() * 10), mPosition.getHeight() - (renderer->getUIScale() * 45)
        },
        0.01f, Color(0.2f)
    );

    // If not editing, draw name of the material
    if (!mIsEditing && mpMaterial)
    {
        renderer->addText(
            mpMaterial->getName(),
            mPosition.getOffset() + smath::vec2(renderer->getUIScale() * 10, renderer->getUIScale() * 55), 0.04f, renderer->getTextSize(TEXT_NORMAL),
            Color(1), TEXT_H_LEFT, TEXT_V_TOP
        );
    }

    // Looping through each element and drawing it
    for (UIElement* element : mpElements)
    {
        element->drawElement(renderer, 0.05f);
    }
}

// Override function definition (EventListener)
void MaterialWindow::handleEvent(const Event& event)
{
    // Checking if proper event was received
    if (event.getType() == EVENT_SOFTWARE_TOGGLE_MATERIAL_EDITING)
    {
        // If currently editing, delete edditing elements and spawn in new ones
        if (mIsEditing)
        {
            createMaterialElements();
            mIsEditing = false;
        }
        // Otherwise, spawn in settings elements
        else
        {
            createMaterialSettings();
            mIsEditing = true;
        }
    }
    else if (event.getType() == EVENT_SOFTWARE_UPDATE_UNIFORM_TYPE)
    {
        // Casting the event to the propper type
        const IntValueEvent& castEvent = static_cast<const IntValueEvent&>(event);

        // Determing the new value type of the uniform
        UniformType newType = (UniformType)(*mpUniformTypes[castEvent.getIntValue()]);

        // If the material is valid, replace the uniform with a new type
        if (mpMaterial)
        {
            // Checking if the new uniform type is the same as the previous
            if (newType == mpMaterial->getUniformAtIndex(castEvent.getIntValue())->getType())
                return; // Does nothing

            // Replacing the uniform
            mpMaterial->replaceUniformAt(newType, castEvent.getIntValue());

            // Recompilig the material
            mpMaterial->generateGraphicsPipeline();
        }
    }
    else if (event.getType() == EVENT_SOFTWARE_REMOVE_UNIFORM)
    {
        // Casting the event to the propper type
        const IntValueEvent& castEvent = static_cast<const IntValueEvent&>(event);

        // If the material is valid, remove the uniform of the event index
        if (mpMaterial)
        {
            // Removing the uniform
            mpMaterial->removeUniformAt(castEvent.getIntValue());

            // Recreating settings UI
            createMaterialSettings();

            // Recompiling the shader
            mpMaterial->generateGraphicsPipeline();
        }
    }
    else if (event.getType() == EVENT_SOFTWARE_ADD_UNIFORM)
    {
        // Checking if the material is valid
        if (mpMaterial)
        {
            // Adding a basic uniform
            mpMaterial->addBlankUniform();

            // Recreating settings UI
            createMaterialSettings();

            // Recompiling the shader
            mpMaterial->generateGraphicsPipeline();
        }
    }
    else if (event.getType() == EVENT_SOFTWARE_RECOMPILE_MATERIAL)
    {
        // If there is an active material, recompile it!
        if (mpMaterial)
            mpMaterial->generateGraphicsPipeline();
    }
    else if (event.getType() == EVENT_SOFTWARE_MANAGE_UNIFORM_NAMES)
    {
        // If there is an active material, call the manage names function
        if (mpMaterial)
            mpMaterial->manageUniformNames();
    }
}

// Private function for adding events to listen to
void MaterialWindow::addEvents()
{
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_TOGGLE_MATERIAL_EDITING, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_UPDATE_UNIFORM_TYPE, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_REMOVE_UNIFORM, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_ADD_UNIFORM, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_RECOMPILE_MATERIAL, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_MANAGE_UNIFORM_NAMES, this);
}