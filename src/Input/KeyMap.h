#ifndef KEY_MAP
#pragma once

#include "../Events/EventSystem.h"
#include <unordered_map>
#include <fstream>
#include <string>
#include <regex>

const std::unordered_map<std::string, EventType> EVENTCODE_TO_EVENTS =
{
    { "save_project", EVENT_SOFTWARE_SAVE },
    { "exit_software", EVENT_SOFTWARE_CLOSE },
    { "open_project", EVENT_SOFTWARE_OPEN_PROJECT_FILE_DIALOGUE },
    { "close_project", EVENT_SOFTWARE_CLOSE_PROJECT },
};

class KeyMap : public EventListener
{
public:
    // Constructor
    KeyMap() {}

    // Data Management functions
    bool init(std::string keyMapFilePath);

    // Loading a keymap file
    void loadKeyMapFromFile(std::string keyMapFilePath);

    // Override Function
    void handleEvent(const Event& event) override;

private:
    std::unordered_map<uint16_t, std::string> mMappedKeys;
};

#endif // !KEY_MAP
