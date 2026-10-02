#include "TextInput.h"

// Inserting text functions
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

// Selection functions
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

// Typing functions
void TextInput::startTyping()
{
    // Setting typing to true, and storing text
    mTyping = true;
    mStoredText = mInputText;
}
void TextInput::startTyping(const std::string& text)
{
    // Setting typing to true, and storing text
    mTyping = true;
    mStoredText = mInputText = text;
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

    // Handling input
    handleInput();
}