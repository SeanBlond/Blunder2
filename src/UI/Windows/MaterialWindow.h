#ifndef MATERIAL_WINDOW
#pragma once

#include "../UIWindows.h"
#include "../../Material/Material.h"

class MaterialWindow : public UIWindow, public EventListener
{
public:
    // Constructor(s)
    MaterialWindow(Material* material, float width, float height, float xoffset, float yoffset)
        : mpMaterial(material), UIWindow(width, height, xoffset, yoffset, "Material") { addEvents(); }
    MaterialWindow(Material* material)
        : mpMaterial(material), UIWindow(0, 0, 0, 0, "Material") { addEvents(); }
    ~MaterialWindow() { }

    // Functions for generating UI
    void changeMaterial(Material* material);
    void createMaterialElements();
    void createMaterialSettings();
    void updateElementPositions();
    UIElement* createElementFromUniform(Uniform* uniform);
    void createSettingsFromUniform(Uniform* uniform, int uniformIndex, float uiScale, float& yPos);

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

    // Override functions (EventListener)
    void handleEvent(const Event& event) override;

private:
    Material* mpMaterial = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE

    // Variables for editting uniforms/materials
    bool mIsEditing = false;
    const std::vector<std::string> UNIFORM_OPTIONS =
    {
        "int",
        "float",
        "float2",
        "float3",
        "float4",
        "color",
    };
    std::vector<int*> mpUniformTypes;
    Uniform* newUniformFromType();

    // Private function for adding events to listen to
    void addEvents();
};

#endif // !MATERIAL_WINDOW
