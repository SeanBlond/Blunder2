#ifndef COLOR_WINDOW
#pragma once

#include "../../UIWindows.h"

class ColorWindow : public UIWindow
{
public:
    // Constructor(s)
    ColorWindow(Color* color, float width, float height, float xoffset, float yoffset)
        : mpMaterial(material), UIWindow(width, height, xoffset, yoffset, "Material") {
        addEvents();
    }
    ColorWindow(Material* material)
        : mpMaterial(material), UIWindow(0, 0, 0, 0, "Material") {
        addEvents();
    }
    ~ColorWindow() { }

    // Function for creating elements
    void CreateUIFromSelected();

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

private:
    Color* interactingColor;
    std::vector<std::string> colorMode = { "RGBA", "HSVA" };
    int currentColorMode = 1;
    smath::vec4 colorData;
    std::string hexCode;
    std::string storedHexCode;
};


#endif // !COLOR_WINDOW