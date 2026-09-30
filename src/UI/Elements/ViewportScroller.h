#pragma once
#include "../UIElement.h"

class ViewportWindow;

class ViewportScroller : public UIElement
{
public:
    ViewportScroller(UIQuad quad, ViewportWindow* viewport) : mpViewport(viewport), UIElement(quad, UI_FLOAT_SLIDER) { generateInteractable(); }
    ~ViewportScroller() {}

    // Functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void onHold(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override {} // Does nothing 

private:
    ViewportWindow* mpViewport; // DO NOT DEALLOCATE IN THIS CLASS
    smath::ivec2 mInitialPos;
    smath::vec2 mInitialOffset;
};