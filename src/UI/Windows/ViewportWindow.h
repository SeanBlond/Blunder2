#ifndef VIEWPORT_WINDOW
#pragma once

#include "../UIWindows.h"

class ViewportWindow : public UIWindow
{
public:
    // Constructor(s)
    ViewportWindow(float width, float height, float xoffset, float yoffset)
        : UIWindow(width, height, xoffset, yoffset, "Viewport") {}
    ViewportWindow()
        : UIWindow(0, 0, 0, 0, "Test") {}

    // Element related functions
    void updateElements();

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

private:
    UIQuad mDisplayQuad;
};

#endif // !TEST_WINDOW
