#include "RecentProjectManager.h"

// Management function definitions
bool RecentProjectManager::init(const std::string& recentProjectsFilepath)
{
    // Adding event listener
    EventSystem::getInstance()->addListener(EVENT_SOFTWARE_OPEN_PROJECT_FILE_DIALOGUE, this);

    // Storing the filepath
    mRecentProjectsFilepath = recentProjectsFilepath;

    // Loading in the filepaths
    loadRecentProjects();

    // Return true for succesful initialization
    return true;
}
void RecentProjectManager::cleanup()
{
    saveRecentProjects();
}

// Loading/Saving function defintions
void RecentProjectManager::loadRecentProjects()
{
    // Creating the input file for the recent projects
    std::ifstream inputFile(mRecentProjectsFilepath);

    // If the file failed to load, return out
    if (!inputFile)
    {
        std::cout << "Failed to read recent project file from: " << mRecentProjectsFilepath << std::endl;
        return;
    }

    // Attempting to read projects (with a max of 5)
    int projectsRead = 0;
    while (!inputFile.eof() && projectsRead < MAX_RECENT_AMOUNT)
    {
        // Reading string
        std::string tempString;
        std::getline(inputFile, tempString);

        // Determing if the filepath actually exists
        std::filesystem::path tempPath(tempString);
        if (!std::filesystem::exists(tempPath))
        {
            std::cout << "Filepath \"" << tempString << "\" does not exist, skipping" << std::endl;
            continue;
        }

        // Adding the string to the vector
        mProjectFilepaths.push_back(tempString);

        // Increasing projects read amount
        projectsRead++;
    }
}
void RecentProjectManager::saveRecentProjects()
{
    // Defining the file that information will be saved to
    std::ofstream saveFile(mRecentProjectsFilepath);

    // Defining amount of projects to save
    int projectsToSave = smath::min((int)mProjectFilepaths.size(), MAX_RECENT_AMOUNT);

    // Looping through all projects filepath and saving them to the file
    for (int i = 0; i < projectsToSave; i++)
    {
        // Outputting the filepath
        saveFile << mProjectFilepaths[i];

        // Adding new line (on every one except the last)
        if (i != projectsToSave - 1)
            saveFile << std::endl;
    }
}

// Project reorgnization function definitions
void RecentProjectManager::moveToFront(int index)
{
    // Clamping index
    index = smath::clamp(index, 0, (int)mProjectFilepaths.size() - 1);

    // Removing the project at index, then inserting it at the front
    std::string storedProjectPath = mProjectFilepaths[index];
    mProjectFilepaths.erase(mProjectFilepaths.begin() + index);
    mProjectFilepaths.insert(mProjectFilepaths.begin(), storedProjectPath);

}
void RecentProjectManager::addToFront(const std::string& filepath)
{
    // INsertting the filepath
    mProjectFilepaths.insert(mProjectFilepaths.begin(), filepath);
}

// New project function definitions
void RecentProjectManager::openFileDialogue()
{
    // Defining the file fitler
    SDL_DialogFileFilter shakeProjectFilters[] =
    {
        { "Shake Projects",  "shake" }
    };

    // Opening the file dialogue
    SDL_ShowOpenFileDialog(
        openFileCallback,
        this,
        GraphicsDevice::getInstance()->getWindow(),
        shakeProjectFilters,
        1,
        nullptr,
        false
    );
}
void RecentProjectManager::openFile(std::string filepath)
{
    // Adding the file as the first element of the recent projects
    mProjectFilepaths.insert(mProjectFilepaths.begin(), filepath);

    // Sending out an event to open to (newly added) first element of the recent projects
    EventSystem::getInstance()->fire(IntValueEvent(EVENT_SOFTWARE_PROJECT_OPEN, 0));
}

// Getter definitions
std::string RecentProjectManager::getProjectFileNameAtIndex(int index) const
{
    // Clamping index
    index = smath::clamp(index, 0, (int)mProjectFilepaths.size() - 1);
    
    // Creating a filepath from the filename
    std::filesystem::path tempPath(mProjectFilepaths[index]);

    // Returning the filename
    return tempPath.filename().string();
}

// Event override function definition
void RecentProjectManager::handleEvent(const Event& event)
{
    // Ensuring the proper event was received
    if (event.getType() == EVENT_SOFTWARE_OPEN_PROJECT_FILE_DIALOGUE)
    {
        // Calling the open file function
        openFileDialogue();
    }
}

// Open file callback function definition
void SDLCALL openFileCallback(void* userdata, const char* const* filelist, int filter)
{
    // Checking if there were eny errors with the file selection
    if (!filelist)
    {
        std::cout << "An error occured: " << SDL_GetError() << std::endl;
        return;
    }
    else if (!*filelist)
        return;

    // Casting the user data to a RecentProjectManager instance
    RecentProjectManager* instance = static_cast<RecentProjectManager*>(userdata);

    // Calling the open function of the instance on the first filelist element
    instance->openFile(*filelist);
}