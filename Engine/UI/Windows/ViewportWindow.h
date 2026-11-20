#ifndef VIEWPORT_WINDOW
#pragma once

#include "../UIWindows.h"

class ViewportWindow : public UIWindow, public EventListener
{
public:
    // Constructor(s)
    ViewportWindow(float width, float height, float xoffset, float yoffset)
        : UIWindow(width, height, xoffset, yoffset, "Viewport") { addEvents(); }
    ViewportWindow()
        : UIWindow(0, 0, 0, 0, "Viewport") { addEvents(); }

    // Element related functions
    void createElements();

    // Data management functions
    void addEvents();
    smath::vec2 getRelativePosition(smath::vec2 pos);
    smath::vec2 getViewPosition() const { return mViewportPosition; }
    void setViewPosition(const smath::vec2& position) { this->mViewportPosition = position; }
    void setViewScaleValue(const float& scaleValue) { this->mViewportScaleValue = scaleValue; }
    void setViewScale(const float& scale);

    // Override functions (UIWindow)
    void OpenWindow() override;
    void CloseWindow() override;
    void ResizeWindow() override;
    void DrawWindow(GraphicsRenderer* renderer) override;

    // Override functions (EventListener)
    void handleEvent(const Event& event) override;

private:
    UIQuad mViewQuad;
    float mSensitivity = 0.1f;
    float mViewportScaleValue = 1.0f;
    smath::vec2 mViewportPosition;
    float calculateViewportScale();
};

#endif // !TEST_WINDOW
