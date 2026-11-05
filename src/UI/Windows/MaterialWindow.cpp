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

    // TODO: Add a text entry for the material name

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
    }

    // Returning the (potentially) newly created elements
    return outputElement;
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

    // Looping through each element and drawing it
    for (UIElement* element : mpElements)
    {
        element->drawElement(renderer, 0.05f);
    }
}