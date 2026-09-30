#ifndef SHAKE
#pragma once

#include "../UI/UI.h"
#include "../Input/Input.h"
#include "../Input/KeyMap.h"
#include "../Logo/Logo.h"
#include "../Runtime/Project/ProjectManager.h"
#include "../Runtime/Project/RecentProjectManager.h"
#include "../Runtime/Time.h"
#include "../graphics/GraphicsDevice.h"
#include "../graphics/ShaderCross.h"
#include "../Runtime/WindowManagement.h"


// Class that contains the underlying architecture and software loop (singleton)
class Blunder : public EventListener
{
public:
	// Singleton Instance Functions
	static Blunder* createInstance();
	static Blunder* getInstance();
	static void destroyInstance();

	// Management Functions
	bool init(const unsigned int& width = 1600, const unsigned int& height = 900);
	void runLoop();
	void cleanup();

	// Event Handling Function
	void handleEvent(const Event& event) override;

	// Getters
	ProjectManager* getProjectManager() { return &mProjectManager; }
	RecentProjectManager* getRecentProjectManager() { return &mRecentProjectManager; }

private:
	// Private Constructor & Deconstructor
	Blunder() {}
	~Blunder() { cleanup(); }

	// Singleton Instance
	static Blunder* mpInstance;

	// Software Managers
	WindowManager mWindowManager;
	InputSystem mInputSystem;
	ProjectManager mProjectManager;
	RecentProjectManager mRecentProjectManager;
	KeyMap mUniversalKeyMap;

	// Constant filepaths
	const std::string UI_TEXTURE_ATLAS_COORDS = "resources/UI/UITextureAtlasCoords.txt";
	const std::string UI_TEXTURE_ATLAS_IMAGE =  "resources/UI/UITextureAtlas.png";
	const std::string TEXT_TEXTURE_ATLAS_COORDS = "resources/UI/Lato-Regular-Bitmap.fnt";
	const std::string TEXT_TEXTURE_ATLAS_IMAGE =  "resources/UI/Lato-Regular-Bitmap.png";
	const std::string UNIVERSAL_KEYMAP_FILE =  "resources/universal.keymap";
	const std::string RECENT_PROJECTS_FILE =  "resources/recent_projects.txt";

	bool mRunning = false;
};

#endif // !SHAKE
