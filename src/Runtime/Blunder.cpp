#include "Blunder.h"

// Setting the mpInstance to be null
Blunder* Blunder::mpInstance = nullptr;

// Singleton Instance Functions Definitions
Blunder* Blunder::createInstance()
{
    // If the instance already exists, destroy it so it can be recreated
    if (mpInstance)
        destroyInstance();

    // Creating the instance
    mpInstance = new Blunder();

    // Returning the instance
    return mpInstance;
}
Blunder* Blunder::getInstance()
{
    // Creating the instance if it's not already
    if (!mpInstance)
        createInstance();

    // Returning the instance
    return mpInstance;
}
void Blunder::destroyInstance()
{
    // Deleting the instance and setting it to null
    delete mpInstance;
    mpInstance = nullptr;
}

// Management Functions Definitions
bool Blunder::init(const unsigned int& width, const unsigned int& height)
{
    // Initializing the various managers (that need to be initted)
    bool successfulInit = true;
    successfulInit &= GraphicsDevice::createInstance()->init(
        width, height, "Blunder - v0.1", 
        UI_TEXTURE_ATLAS_COORDS, UI_TEXTURE_ATLAS_IMAGE, TEXT_TEXTURE_ATLAS_COORDS, TEXT_TEXTURE_ATLAS_IMAGE);
    successfulInit &= ShaderCross::createInstance()->init();
    successfulInit &= Time::init();
    successfulInit &= mInputSystem.init();
    successfulInit &= mProjectManager.init();
    successfulInit &= mRecentProjectManager.init(RECENT_PROJECTS_FILE);
    successfulInit &= mUniversalKeyMap.init(UNIVERSAL_KEYMAP_FILE);
    successfulInit &= mWindowManager.init(width, height);

    // Returning false if failed to init
    if (!successfulInit)
        return false;

    // Setting up events to listen for 
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_CLOSE, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_SAVE, this);
    EventSystem::getInstance()->addListener(EVENT_WINDOW_RESIZE, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_PROJECT_OPEN, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_CREATE_NEW_PROJECT, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_SET_ROOT_WINDOW, this);
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_CLOSE_PROJECT, this);

    // Returning true for succesful initiailization
    return true;
}
void Blunder::runLoop()
{
    // Updating the running bool
    mRunning = true;

    // Actual software loop
    while (mRunning)
    {
        // Updating Time
        Time::UpdateTime();

        // Updating Input
        mInputSystem.updateInput();

        // Drawing Windows
        mWindowManager.drawWindows(GraphicsDevice::getInstance()->getRenderer());

        // Drawing the Graphics Renderer
        GraphicsDevice::getInstance()->getRenderer()->drawBuffers();
    }
}
void Blunder::cleanup()
{

}

// Project management function definitions

// Event Handling Function
void Blunder::handleEvent(const Event& event)
{
    // Doing different things based on the event type
    if (event.getType() == EVENT_SOFTWARE_CLOSE)
    {
        // Closing out of the software
        mRunning = false;
    }
    else if (event.getType() == EVENT_SOFTWARE_SAVE)
    {
        // Saving the project
        mProjectManager.saveProject();
    }
    else if (event.getType() == EVENT_WINDOW_RESIZE)
    {
        // Casting the event to be a window resiae event
        WindowResizeEvent castEvent = static_cast<const WindowResizeEvent&>(event);

        // Resizing the UI Windows
        mWindowManager.updateWindows(castEvent.getSize());

        // Updating the Graphics Device dimensions
        GraphicsDevice::getInstance()->setDisplayDimensions(castEvent.getSize());

        // Udpating the renderer depth texture
        GraphicsDevice::getInstance()->getRenderer()->updateDepthTexture(castEvent.getSize());
    }
    else if (event.getType() == EVENT_SOFTWARE_PROJECT_OPEN)
    {
        // Casting the event to the propper type
        IntValueEvent castEvent = static_cast<const IntValueEvent&>(event);

        // Opening a project with the included index
        //openProjectFromIndex(castEvent.getIntValue());
    }
    else if (event.getType() == EVENT_SOFTWARE_SET_ROOT_WINDOW)
    {
        // Casting the event to the propper type
        RootWindowChangeEvent castEvent = static_cast<const RootWindowChangeEvent&>(event);

        // Calling the change window function and passing through the option
        mWindowManager.changeRootWindow(castEvent.getWindowOption());
    }
    else if (event.getType() == EVENT_SOFTWARE_CREATE_NEW_PROJECT)
    {
        // TODO: Check if any photos are being dragged in

        // Creating the new project
        //createNewProject();
    }
    else if (event.getType() == EVENT_SOFTWARE_CLOSE_PROJECT)
    {
        // Only closing if there's an active project
        if (!mProjectManager.getProjectActive())
            return;

        // Closing the project
        mProjectManager.closeProject();

        // Resetting back to the load project window
        mWindowManager.changeRootWindow(WINDOW_PROJECT_LOADER);
    }
}