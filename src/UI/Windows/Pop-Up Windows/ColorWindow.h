#ifndef COLOR_WINDOW
#pragma once

#include "../../UIWindows.h"

class ColorWindow : public UIWindow
{
public:
    // Constructor(s)
    ColorWindow(Color* color, float width, float height, float xoffset, float yoffset)
        : mpColor(color), UIWindow(width, height, xoffset, yoffset, "Color") {}
    ColorWindow(Color* color)
        : mpColor(color), UIWindow(0, 0, 0, 0, "Color") {}
    ~ColorWindow() {}

    // Function for creating elements
    void CreateUIFromColor();

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

private:
    Color* mpColor;
    std::vector<std::string> colorMode = { "RGBA", "HSVA" };
    int currentColorMode = 1;
    smath::vec4 colorData;
    std::string hexCode;
    std::string storedHexCode;
};


#endif // !COLOR_WINDOW