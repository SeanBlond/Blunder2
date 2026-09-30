#ifndef PROJECT_MANAGER
#pragma once

#include <filesystem>
#include <fstream>
#include <vector>
#include "../../Events/EventSystem.h"
#include "../../GameObject/GameObject.h"

class ProjectManager
{
public:
    // Constructors & Deconstructor
    ProjectManager() {}
    ~ProjectManager() { cleanup(); }

    // Management functions
    bool init();
    void cleanup();

    // Project management functions
    bool loadProject(const std::string& projectFileName);
    void createNewProject();
    void closeProject();
    void saveProject();
    void saveNewProject();

    // Getters
    bool getProjectActive() const { return mProjectActive; }
    std::string getProjectFilePath() const { return mProjectFilePath; }
    std::string getProjectName() const { return mStoredProjectName; }
    GameObject* getActiveObject() { return &mTempObject; }

    // Setters
    void setProjectFilepath(const std::string& filepath) { mProjectFilePath = filepath; }

private:
    // Project data
    std::string mProjectFilePath = "";
    std::string mStoredProjectName = "";
    bool mProjectActive = false;
    GameObject mTempObject;
};

// Max amount of images that can be loaded at once
static const int MAX_IMAGES_TO_LOAD = 100;

// Save file callback function
static void SDLCALL saveFileCallback(void* userdata, const char* const* filelist, int filter);

#endif