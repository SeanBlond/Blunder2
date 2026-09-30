#include "ProjectManager.h"
#include "../../graphics/GraphicsDevice.h"
#include "../Blunder.h"

// Management function definitions
bool ProjectManager::init()
{
    // Return true for succesful initialization
    return true;
}
void ProjectManager::cleanup()
{
    // Closing the project
    closeProject();
}

// Project management function definitions
bool ProjectManager::loadProject(const std::string& projectFileName)
{
    // Closing old project
    closeProject();

    // Storing the new filepath
    mProjectFilePath = projectFileName;

    // Loading the file
    std::ifstream projectFile(projectFileName);

    // If the file fails to load, return false
    if (!projectFile)
    {
        std::cout << "Failed to load the project file \"" << projectFileName << '\"' << std::endl;
        projectFile.close();
        return false;
    }

    // Setting the stored name to be the file name (without the pathways)
    std::filesystem::path tempPath(mProjectFilePath);
    mStoredProjectName = tempPath.filename().string();

    // Closing the file
    projectFile.close();

    // Setting project active to be true
    mProjectActive = true;

    // Return true for succesful loading
    return true;
}
void ProjectManager::createNewProject()
{
    // Closing old project
    closeProject();

    // Defining null project filepath
    mProjectFilePath = "";

    // Setting project active to be true
    mProjectActive = true;
}
void ProjectManager::closeProject()
{
    // Setting project active to be false
    mProjectActive = false;
}
void ProjectManager::saveProject()
{
    // Checking if their is even a project to save
    if (!mProjectActive)
        return;

    // Checking if the project has a path to be saved to
    if (mProjectFilePath == "")
    {
        // Project has no filepath, creating new one instead
        saveNewProject();

        // Returning out of this save function
        return;
    }

    // Creating an output object to write project data to
    std::string outputFileName = mProjectFilePath;
    std::cout << "Saving to: " << mProjectFilePath << std::endl;
    std::ofstream outputFile(outputFileName);

    // Closing file
    outputFile.close();
}
void ProjectManager::saveNewProject()
{
    // Defining the file fitler
    SDL_DialogFileFilter shakeProjectFilters[] =
    {
        { "Blunder Projects",  "blund" }
    };

    // Opening the file dialogue
    SDL_ShowSaveFileDialog(
        saveFileCallback, this,
        GraphicsDevice::getInstance()->getWindow(),
        shakeProjectFilters, 1, nullptr);
}

// Save file callback function definition
void SDLCALL saveFileCallback(void* userdata, const char* const* filelist, int filter)
{
    // Checking if there were eny errors with the file selection
    if (!filelist)
    {
        std::cout << "An error occured: " << SDL_GetError() << std::endl;
        return;
    }
    else if (!*filelist)
        return;

    // Ensuring the last part of the filename is ".shake"
    std::string savePath = (*filelist);
    if (savePath.substr(savePath.size() - 6, 6) != ".shake")
        savePath += ".shake";

    // Casting the user data to a ProjectManager instance
    ProjectManager* instance = static_cast<ProjectManager*>(userdata);

    // Calling the open function of the instance on the first filelist element
    instance->setProjectFilepath(savePath);
    instance->saveProject();

    // Adding the filepath to the recent projects
    Blunder::getInstance()->getRecentProjectManager()->addToFront(savePath);
}