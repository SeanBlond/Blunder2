#pragma once
#include "../UIElement.h"

class Text : public UIElement
{
public:
    Text(UIQuad quad, std::string text, float size, Color color = Color(1), HorizontalTextAlign horizontalAlignment = TEXT_H_LEFT, VerticalTextAlign verticalAlignment = TEXT_V_MIDDLE)
        : mText(text), mSize(size), mHorizAlign(horizontalAlignment), mVertAlign(verticalAlignment), mColor(color), UIElement(quad, UI_TEXT) {}

    // Getters
    std::string getText() const { return mText; }
    float getSize() const { return mSize; }
    HorizontalTextAlign getHorizAlign() const { return mHorizAlign; }
    VerticalTextAlign getVertAlign() const { return mVertAlign; }
    Color getColor() const { return mColor; }

    // Setters
    void setText(const std::string& text) { this->mText = text; }
    void setSize(const float& size) { this->mSize = size; }
    void setHorizAlign(const HorizontalTextAlign& horizAlign) { this->mHorizAlign = horizAlign; }
    void setVertAlign(const VerticalTextAlign& vertAlign) { this->mVertAlign = vertAlign; }
    void setColor(const Color& color) { this->mColor = color; }

    // Override functions
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    // Text Data
    std::string mText;
    float mSize;
    HorizontalTextAlign mHorizAlign;
    VerticalTextAlign mVertAlign;
    Color mColor;
};