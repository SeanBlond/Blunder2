#include "TextInput.h"

// Inserting text function definitions
void TextInput::insert(char character)
{
    // Removing selected text if there is any
    if (mSelecting)
        remove();

    // Resetting cursor if needed
    if (mInputText.length() <= 0)
        mCursorPos = 0;

    // Inserting text
    if (mCursorPos != 0)
        mInputText.insert(mInputText.end() + mCursorPos, character);
    else
        mInputText.push_back(character);
}
void TextInput::insert(const char* string)
{
    // Removing selected text if there is any
    if (mSelecting)
        remove();

    // Resetting cursor if needed
    if (mInputText.length() <= 0)
        mCursorPos = 0;

    // INserting the string
    if (mCursorPos != 0)
        mInputText.insert(mInputText.length() + mCursorPos, string);
    else
        mInputText += string;
}
void TextInput::remove()
{
    if (mInputText.length() <= 0)
        return;

    // If not selecting, removing character at cursor
    if (!mSelecting)
    {
        if (mCursorPos == 0)
            mInputText.pop_back();
        else if (mCursorPos > -(int)mInputText.length())
            mInputText.erase(mInputText.end() + mCursorPos - 1);
    }
    // Selecing, erasing text from cursor pos to select pos
    else
    {
        if (mCursorPos == 0)
            mInputText.erase(mInputText.begin() + mSelectPos, mInputText.end());
        else
        {
            if (mInputText.length() + mCursorPos < mSelectPos)
            {
                mInputText.erase(mInputText.end() + mCursorPos, mInputText.begin() + mSelectPos);
                mCursorPos += mSelectPos - ((int)mInputText.length() + mCursorPos);
            }
            else
                mInputText.erase(mInputText.begin() + mSelectPos, mInputText.end() + mCursorPos);
        }

        // Resetting selection
        resetSelection();
    }

    // Clamping Cursor after deletion
    setCursor(mCursorPos);
}

// Cursor fuctions
void TextInput::setCursor(const int& cursor)
{ 
    this->mCursorPos = smath::clamp(cursor, -(int)(mInputText.length()), 0); 
}
void TextInput::shiftCursor(const int& amount)
{
    // Clamping the shifted cursor amount
    setCursor(mCursorPos + amount);
}

// Selection function definitions
void TextInput::setSelection(const int& position) 
{ 
    this->mSelectPos = smath::clamp(position, 0, (int)mInputText.length()); 
}
void TextInput::resetSelection()
{
    // Resetting selection values
    mSelecting = false;
    mSelectPos = 0;
}
void TextInput::updateSelection()
{
    if (!mSelecting)
    {
        mSelecting = true;
        setSelection((int)mInputText.length() + mCursorPos);
    }
}
void TextInput::selectAll()
{
    setCursor(0);
    mSelecting = true;
    setSelection(-(int)mInputText.length());
    updateSelection();
}
std::string TextInput::returnSelected() const
{
    std::string substring = "";
    if (mSelecting)
    {
        if (mInputText.length() + mCursorPos < mSelectPos)
            substring = mInputText.substr(mInputText.length() + mCursorPos, (mSelectPos - mInputText.length() + mCursorPos));
        else
            substring = mInputText.substr(mSelectPos, (mInputText.length() + mCursorPos - mSelectPos));

    }
    return substring;
}
std::string TextInput::cutSelected()
{
    std::string substring = returnSelected();

    if (mSelecting)
        remove();

    return substring;
}

// Typing function definitions
void TextInput::startTyping()
{
    // Setting typing to true, and storing text
    mTyping = true;
    mStoredText = mInputText;
    resetSelection();
}
void TextInput::startTyping(const std::string& text)
{
    // Setting typing to true, and storing text
    mTyping = true;
    mStoredText = mInputText = text;
    resetSelection();
}
void TextInput::inputText(const std::string& text)
{
    // Inserting the text
    insert(text[0]);
}
void TextInput::endTyping(bool saveText)
{
    // Stopping typing
    mTyping = false;

    // Changing input text depending on if it should be saved
    if (!saveText)
        mInputText = mStoredText;
    else
        mStoredText = mInputText;

    // Handling input
    handleInput();
}

// Drawing function definition
void TextInput::drawTextInput(GraphicsRenderer* renderer, smath::vec2 pos, float layerOffset, Color color,
    HorizontalTextAlign horizontalAlignment, VerticalTextAlign verticalAlignment)
{
    // Rendering Cursor & Selection
    if (mTyping)
    {
        // Cursor
        smath::vec2 cursorPos = renderer->getPositionInText(mInputText, getCursorPos(), pos, renderer->getTextSize(TEXT_NORMAL), horizontalAlignment, verticalAlignment);
        cursorPos -= smath::vec2(0.0f, renderer->getTextSize(TEXT_NORMAL) * (verticalAlignment == TEXT_V_BOTTOM ? 0.0f : (verticalAlignment == TEXT_V_MIDDLE ? 0.5f : 1.0f)));
        renderer->addRectangle(cursorPos, smath::vec2(3 * renderer->getUIScale(), renderer->getTextSize(TEXT_NORMAL)), layerOffset + 0.01f, Color(0.8f));

        // Selection
        if (mSelecting)
        {
            smath::vec2 selectPos = renderer->getPositionInText(mInputText, mSelectPos, pos, renderer->getTextSize(TEXT_NORMAL), horizontalAlignment, verticalAlignment);
            selectPos -= smath::vec2(0.0f, renderer->getTextSize(TEXT_NORMAL) * (verticalAlignment == TEXT_V_BOTTOM ? 0.0f : (verticalAlignment == TEXT_V_MIDDLE ? 0.5f : 1.0f)));
            float selectSize = abs(cursorPos.x - selectPos.x);
            smath::vec2 selectStartPos = smath::vec2(smath::min(selectPos.x, cursorPos.x), selectPos.y);
            renderer->addRectangle(selectStartPos, smath::vec2(selectSize, renderer->getTextSize(TEXT_NORMAL)), layerOffset, Color(0.6f, 0.6f, 1.0f));
        }
    }

    // Rendering Text
    renderer->addText(mInputText, pos, layerOffset + 0.02f, renderer->getTextSize(TEXT_NORMAL), Color(1), horizontalAlignment, verticalAlignment);
}