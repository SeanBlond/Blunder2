#ifndef UNIFORM_TEXT_ENTRY
#pragma once

#include "../UIElement.h"
#include "../../Material/Uniforms.h"

class UniformTextEntry : public UIElement, public TextInput
{
public:
    UniformTextEntry(UIQuad quad, Uniform* uniform)
        : mpUniform(uniform), UIElement(quad, UI_TEXT_ENTRY), TextInput(uniform->getUniformName()) { generateInteractable(); }

    // Override functions
    void generateInteractable() override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;
    TextInput* getTextInput() override { return this; }
    void handleInput() override;

private:
    Uniform* mpUniform = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE
};

#endif // !TEXT_ENTRY
