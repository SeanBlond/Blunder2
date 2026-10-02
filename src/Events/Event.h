#pragma once

#include <smath/smath.h>
#include <SDL3/SDL.h>

// Enum for the different types of events that will be used
enum EventType
{
    // Null Event (you know how it is)
	INVALID_EVENT = -1,

    // Events for User Interaction
    EVENT_KEY_DOWN,
    EVENT_KEY_UP,
    EVENT_TEXT_INPUT,
    EVENT_MOUSE_DOWN,
    EVENT_MOUSE_UP,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_SCROLL,
    EVENT_FILE_DROP,

    // Window events
    EVENT_WINDOW_RESIZE,

    // Software events
    EVENT_SOFTWARE_CLOSE,
    EVENT_SOFTWARE_SAVE,
    EVENT_SOFTWARE_VIEWPORT_RESET,
    EVENT_SOFTWARE_PROJECT_OPEN,
    EVENT_SOFTWARE_CREATE_NEW_PROJECT,
    EVENT_SOFTWARE_RESET_INTERACTION_ELEMENTS,
    EVENT_SOFTWARE_OPEN_PROJECT_FILE_DIALOGUE,
    EVENT_SOFTWARE_SET_ROOT_WINDOW,
    EVENT_SOFTWARE_CLOSE_PROJECT,
};


// * ------------------ *
// |  Event Base Class  |
// * ------------------ *
class Event
{
public:
    Event(EventType type) : mType(type) {}
    virtual ~Event() {}

    inline EventType getType() const { return mType; }

private:
    EventType mType;
};



// * ------------------- *
// |  User Input Events  |
// * ------------------- *
struct KeystrokeData
{
    KeystrokeData(uint8_t scancode, bool ctrl = false, bool shift = false, bool alt = false)
    {
        // Creating the keycode with the scancode number
        uint16_t baseScancode = (uint16_t)scancode;

        // Creating a mask with the modifiers
        uint16_t modifierMask =
            ((uint16_t)ctrl  << 9) |
            ((uint16_t)shift << 8) |
            ((uint16_t)alt   << 7);

        // Combing the two together to set the keycode
        mKeycode = baseScancode | modifierMask;
    }

    uint16_t mKeycode;
};

class KeyDownEvent : public Event
{
public:
    // Constructor
    KeyDownEvent(KeystrokeData keyData) : mKeyData(keyData), Event(EVENT_KEY_DOWN) {}

    // Getters
    KeystrokeData getKeyData() const { return mKeyData; }

private:
    KeystrokeData mKeyData;
};
class KeyUpEvent : public Event
{
public:
    // Constructor
    KeyUpEvent(KeystrokeData keyData) : mKeyData(keyData), Event(EVENT_KEY_UP) {}

    // Getters
    KeystrokeData getKeyData() const { return mKeyData; }

private:
    KeystrokeData mKeyData;
};
class TextInputEvent : public Event
{
public:
    TextInputEvent(std::string text) : mText(text), Event(EVENT_TEXT_INPUT) {}

    // Getters
    std::string getText() const { return mText; }

private:
    std::string mText;
};

struct MouseClickData
{
    SDL_MouseButtonFlags mButton;
    smath::ivec2 mPosition;

};
class MouseDownEvent : public Event
{
public:
    // Constructor
    MouseDownEvent(SDL_MouseButtonFlags button, smath::ivec2 position) : mClickData({ button, position }), Event(EVENT_MOUSE_DOWN) {}
    MouseDownEvent(int button, smath::ivec2 position) : mClickData({ (SDL_MouseButtonFlags)button, position }), Event(EVENT_MOUSE_DOWN) {}

    // Getters
    SDL_MouseButtonFlags getButton() const { return mClickData.mButton; }
    smath::ivec2 getPosition() { return mClickData.mPosition; };
    MouseClickData getClickData() { return mClickData; }

private:
    MouseClickData mClickData;
};
class MouseUpEvent : public Event
{
public:
    // Constructor
    MouseUpEvent(SDL_MouseButtonFlags button, smath::ivec2 position) : mClickData({button, position}), Event(EVENT_MOUSE_UP) {}
    MouseUpEvent(int button, smath::ivec2 position) : mClickData({ (SDL_MouseButtonFlags)button, position }), Event(EVENT_MOUSE_UP) {}

    // Getters
    SDL_MouseButtonFlags getButton() const { return mClickData.mButton; }
    smath::ivec2 getPosition() const { return mClickData.mPosition; };
    MouseClickData getClickData() const { return mClickData; }

private:
    MouseClickData mClickData;
};
class MouseMoveEvent : public Event
{
public:
    // Constructor
    MouseMoveEvent(smath::ivec2 position, smath::ivec2 delta, SDL_MouseButtonFlags button) : mPosition(position), mDelta(delta), mButton(button), Event(EVENT_MOUSE_MOVE) {}
    MouseMoveEvent(smath::ivec2 position, smath::ivec2 delta, int button) : mPosition(position), mDelta(delta), mButton((SDL_MouseButtonFlags)button), Event(EVENT_MOUSE_MOVE) {}

    // Getters
    SDL_MouseButtonFlags getButton() { return mButton; }
    smath::ivec2 getPosition() { return mPosition; };
    smath::ivec2 getDelta() { return mDelta; };
    MouseClickData getClickData() { return { mButton, mPosition }; }

private:
    SDL_MouseButtonFlags mButton;
    smath::ivec2 mPosition;
    smath::ivec2 mDelta;
};
class MouseScrollEvent : public Event
{
public:
    // Constructor
    MouseScrollEvent(smath::vec2 scroll, smath::ivec2 position, bool flipped) : mScroll(scroll), mPosition(position), mFlipped(flipped), Event(EVENT_MOUSE_SCROLL) {}

    // Getters
    smath::vec2 getScroll() { return mScroll; };
    smath::ivec2 getPosition() { return mPosition; };
    bool getFlipped() { return mFlipped; }

private:
    smath::vec2 mScroll;
    smath::ivec2 mPosition;
    bool mFlipped;
};

class DropFileEvent : public Event
{
public:
    // Constructor
    DropFileEvent(const smath::vec2& position, const std::string& filepath)
        : mPosition(position), mFilepath(filepath), Event(EVENT_FILE_DROP) {}

    // Getters
    smath::vec2 getPosition() const { return mPosition; }
    std::string getFilepath() const { return mFilepath; }

private:
    smath::ivec2 mPosition;
    std::string mFilepath;
};

class WindowResizeEvent : public Event
{
public:
    // Constructor
    WindowResizeEvent(smath::ivec2 windowSize) : mWindowSize(windowSize), Event(EVENT_WINDOW_RESIZE) {}

    // Getters
    smath::ivec2 getSize() { return mWindowSize; }

private:
    smath::ivec2 mWindowSize;
};


// * ------------------------ *
// |  Software Runtime Event  |
// * ------------------------ *
class SoftwareRuntimeEvent : public Event
{
public:
    // Constructor
    SoftwareRuntimeEvent(EventType event) : Event(event) {}
};

// * --------------------- *
// |  Integer Value Event  |
// * --------------------- *
class IntValueEvent : public Event
{
public:
    // Constructor
    IntValueEvent(EventType event, int value) : Event(event), mIntValue(value) {}

    // Getters
    int getIntValue() { return mIntValue; }

private:
    int mIntValue;
};

// * -------------------------- *
// |  Root Window Change Event  |
// * -------------------------- *
enum RootWindowOptions
{
    WINDOW_NONE = -1,
    WINDOW_VIEWPORT,
    WINDOW_PROJECT_LOADER
};
class RootWindowChangeEvent : public Event
{
public:

    // Constructor
    RootWindowChangeEvent(RootWindowOptions windowOption) : Event(EVENT_SOFTWARE_SET_ROOT_WINDOW), mWindowOption(windowOption) {}

    // Getters
    RootWindowOptions getWindowOption() { return mWindowOption; }


private:
    RootWindowOptions mWindowOption;
};