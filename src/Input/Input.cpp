#include "Input.h"

// Input Management Function Dewfinitions
void InputSystem::updateInput()
{
    // Looping through potential events
    while (SDL_PollEvent(&mEvent))
    {
        // Upon event being found, it is converted from SDL to a software-related equivalent (if possible)
        switch (mEvent.type)
        {
        case SDL_EVENT_WINDOW_RESIZED:
            EventSystem::getInstance()->fire(WindowResizeEvent(smath::ivec2(mEvent.window.data1, mEvent.window.data2)));
            break;

        case SDL_EVENT_QUIT:
            EventSystem::getInstance()->fire(SoftwareRuntimeEvent(EVENT_SOFTWARE_CLOSE));
            break;

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            handleSDLKeyEvent(mEvent);
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
        case SDL_EVENT_MOUSE_WHEEL:
            handleSDLMouseInputEvent(mEvent);
            break;

        case SDL_EVENT_MOUSE_MOTION:
            handleSDLMouseMotionEvent(mEvent);
            break;

        case SDL_EVENT_DROP_FILE:
            handleSDLDropEvent(mEvent);
            break;

        default:
            break;
        }
    }
}

// SDL Event Handling Function Definitions
void InputSystem::handleSDLKeyEvent(SDL_Event event)
{
    // Only send out an event if scancode is greater than 115
    if (event.key.scancode > 115)
        return;

    // Storing keystroke data of the event
    KeystrokeData tempData = KeystrokeData(
        event.key.scancode,
        (event.key.mod & SDL_KMOD_CTRL),
        (event.key.mod & SDL_KMOD_SHIFT),
        (event.key.mod & SDL_KMOD_ALT)
    );

    //std::cout << tempData.mKeycode << std::endl;

    if (event.type == SDL_EVENT_KEY_DOWN)
        EventSystem::getInstance()->fire(KeyDownEvent(tempData));
    else if (event.type == SDL_EVENT_KEY_UP)
        EventSystem::getInstance()->fire(KeyUpEvent(tempData));
}
void InputSystem::handleSDLMouseInputEvent(SDL_Event event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        EventSystem::getInstance()->fire(MouseDownEvent(event.button.button, smath::ivec2(event.button.x, event.button.y)));
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        EventSystem::getInstance()->fire(MouseUpEvent(event.button.button, smath::ivec2(event.button.x, event.button.y)));
    else if (event.type == SDL_EVENT_MOUSE_WHEEL)
    {
        EventSystem::getInstance()->fire(MouseScrollEvent(
            smath::vec2(event.wheel.x, event.wheel.y), 
            smath::ivec2(event.wheel.mouse_x, event.wheel.mouse_y),
            (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)));
    }
}
void InputSystem::handleSDLMouseMotionEvent(SDL_Event event)
{
    if (event.type == SDL_EVENT_MOUSE_MOTION)
        EventSystem::getInstance()->fire(MouseMoveEvent(smath::ivec2(event.motion.x, event.motion.y), smath::ivec2(event.motion.xrel, event.motion.yrel), event.motion.state));
}
void InputSystem::handleSDLDropEvent(SDL_Event event)
{
    // Broadcasting a drop file event (this might actually be the coolest thing ever btw)
    EventSystem::getInstance()->fire(
        DropFileEvent(smath::vec2(event.drop.x, event.drop.y), event.drop.data)
    );
}