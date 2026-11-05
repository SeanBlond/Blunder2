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

    // Material isn't null, add elements for name entry, uniforms, and shadercode file switching

}

// Override functions (UIWindow)
void MaterialWindow::OpenWindow()
{

}
void MaterialWindow::CloseWindow()
{

}
void MaterialWindow::ResizeWindow()
{

}
void MaterialWindow::DrawWindow(GraphicsRenderer* renderer)
{

}