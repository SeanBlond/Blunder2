#ifndef TEST_WINDOW
#pragma once

#include "../UIWindows.h"

class TestWindow : public UIWindow
{
public:
    // Constructor(s)
    TestWindow(float width, float height, float xoffset, float yoffset)
        : UIWindow(width, height, xoffset, yoffset, "Test") {}
    TestWindow()
        : UIWindow(0, 0, 0, 0, "Test") {}

    // Element related functions
    void updateElements();

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

private:
};

#endif // !TEST_WINDOW
