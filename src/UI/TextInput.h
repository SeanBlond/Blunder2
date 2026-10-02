#ifndef TEXT_ELEMENT
#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <smath/smath.h>
#include "../Events/Event.h"
#include "../graphics/GraphicsDevice.h"

class TextInput
{
public:
    // Constructor
    TextInput(const std::string& text = "") : mInputText(text), mStoredText(text) {}

    // Getters
    std::string getInputText() const { return mInputText; }
    bool getTyping() const { return mTyping; }
    int getCursorPos() const { return smath::clamp((int)mInputText.size() + mCursorPos, 0, (int)mInputText.size()); }
    int getSelectPos() const { return smath::clamp((int)mInputText.size() + mSelectPos, 0, (int)mInputText.size()); }

    // Inserting text functions
    void insert(char character);
    void insert(const char* string);
    void remove();

    // Cursor fuctions
    void setCursor(const int& cursor);
    void shiftCursor(const int& amount);

    // Selection functions
    void setSelection(const int& position);
    void resetSelection();
    void updateSelection();
    void selectAll();
    std::string returnSelected() const;
    std::string cutSelected();

    // Typing functions
    void startTyping();
    void startTyping(const std::string& text);
    void inputText(const std::string& text);
    void endTyping(bool saveText);

    // Drawing function
    void drawTextInput(GraphicsRenderer* renderer, smath::vec2 pos, float layerOffset, Color color = Color(1),
        HorizontalTextAlign horizontalAlignment = TEXT_H_LEFT, VerticalTextAlign verticalAlignment = TEXT_V_BOTTOM);

    // Virtual function
    virtual void handleInput() = 0;

private:
    std::string mInputText = "";
    std::string mStoredText = "";
    int mCursorPos = 0;
    int mSelectPos = 0;
    bool mTyping = false;        
    bool mSelecting = false;
};

#endif // !TEXT_ELEMENT
