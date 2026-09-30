#ifndef PROJECT_MANAGER
#pragma once

#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#include "../../Events/EventSystem.h"
#include "../../smath/smath.h"
#include "../../graphics/GraphicsDevice.h"

const int MAX_RECENT_AMOUNT = 10;

class RecentProjectManager : public EventListener
{
public:
    // Constructors & Deconstructor
    RecentProjectManager() {}
    ~RecentProjectManager() { cleanup(); }

    // Management functions
    bool init(const std::string& recentProjectsFilepath);
    void cleanup();

    // Loading/Saving functions
    void loadRecentProjects();
    void saveRecentProjects();

    // Project reorgnization functions
    void moveToFront(int index);
    void addToFront(const std::string& filepath);

    // New project functions
    void openFileDialogue();
    void openFile(std::string filepath);

    // Getters
    int getRecentProjectAmount() const { return mProjectFilepaths.size(); }
    std::string getProjectFilepathAtIndex(int index) const { index = smath::clamp(index, 0, (int)mProjectFilepaths.size() - 1); return mProjectFilepaths[index]; }
    std::string getProjectFileNameAtIndex(int index) const;

    // Event override functions
    void handleEvent(const Event& event) override;

private:
    // Recent Projects data
    std::string mRecentProjectsFilepath;
    std::string mOpenedFilepath;
    std::vector<std::string> mProjectFilepaths;
};

// Open file callback function
static void SDLCALL openFileCallback(void* userdata, const char* const* filelist, int filter);

#endif