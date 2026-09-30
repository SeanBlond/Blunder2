#include "KeyMap.h"

// Data Management function definition
bool KeyMap::init(std::string keyMapFilePath)
{
    // Adding event listener
    EventSystem::getInstance()->addListener(EVENT_KEY_DOWN, this);

    // Loading keymap
    loadKeyMapFromFile(keyMapFilePath);

    // Return true for succesful commit
    return true;
}

// Loading a keymap file
void KeyMap::loadKeyMapFromFile(std::string keyMapFilePath)
{
    // Loading the file
    std::ifstream keymapFile(keyMapFilePath);

    // If file fails to load, exit out
    if (!keymapFile)
    {
        std::cout << "Failed to load keymap from file: " << keyMapFilePath << std::endl;
        return;
    }

    // Looping through the file and reading keymaps
    while (!keymapFile.eof())
    {
        // Attempting to read a keycode
        std::string tempString = "";
        keymapFile >> tempString;
        uint16_t tempKeycode = 0;

        // Trying to convert string to a uint16_t keycode
        try
        {
            tempKeycode = std::stoi(tempString);
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Could not convert string to keycode with error: " << e.what() << std::endl;
            return;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "Could not convert string to keycode with error: " << e.what() << std::endl;
            return;
        }

        // If the keycode is valid, read it's result
        std::string tempKeyResult = "";
        keymapFile >> tempKeyResult;

        // Storing both into the mapped keys
        mMappedKeys.insert({ tempKeycode, tempKeyResult });
    }

    // Closing the file
    keymapFile.close();
}

// Override Function definition
void KeyMap::handleEvent(const Event& event)
{
    // Ensuring the propper event type was received
    if (event.getType() != EVENT_KEY_DOWN)
        return;

    // Casting the event
    const KeyDownEvent castEvent = static_cast<const KeyDownEvent&>(event);
    uint16_t keycode = castEvent.getKeyData().mKeycode;

    // If the maps contains the keycode, do something with it
    if (mMappedKeys.contains(keycode) && EVENTCODE_TO_EVENTS.contains(mMappedKeys.at(keycode)))
    {
        // Broadcasting a software event of the keycode's respective type
        EventSystem::getInstance()->fire(SoftwareRuntimeEvent(EVENTCODE_TO_EVENTS.at(mMappedKeys.at(keycode))));
    }
}