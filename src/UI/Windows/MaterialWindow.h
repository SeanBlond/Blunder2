#ifndef MATERIAL_WINDOW
#pragma once

#include "../UIWindows.h"
#include "../../Material/Material.h"

class MaterialWindow : public UIWindow
{
public:
    // Constructor(s)
    MaterialWindow(Material* material, float width, float height, float xoffset, float yoffset)
        : mpMaterial(material), UIWindow(width, height, xoffset, yoffset, "Material") {}
    MaterialWindow(Material* material)
        : mpMaterial(material), UIWindow(0, 0, 0, 0, "Material") {}
    ~MaterialWindow() { }

    // Functions for generating UI
    void changeMaterial(Material* material);
    void createMaterialElements();
    void updateElementPositions();
    UIElement* createElementFromUniform(Uniform* uniform);

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

private:
    Material* mpMaterial = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
};

#endif // !MATERIAL_WINDOW
