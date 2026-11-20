#include <Blunder/Engine.h>

int main(int argc, char* argv[])
{
    // Creating the Blunder Software Instance
    if (!(Blunder::createInstance()->init()))
    {
        // Exiting program if failed initialization
        std::cout << "Failed to initialize Blunder2, exiting program" << std::endl;
    }
    else
    {
        // Running Software loop on succesful initialization
        Blunder::getInstance()->runLoop();
    }

    // Destroying Blunder instance
    Blunder::destroyInstance();

    // Destroying other singletons that worked with Blunder
    ResourceManager::destroyInstance();
    EventSystem::destroyInstance();
    ShaderCross::destroyInstance();
    GraphicsDevice::destroyInstance();

    return 0;
}