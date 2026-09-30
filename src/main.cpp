#include "Runtime/Blunder.h"

int main(int argc, char* argv[])
{
    // Creating EventSystem
    EventSystem::createInstance();

    // Creating the Blunder Software Instance
    Blunder::createInstance();
    if (!(Blunder::getInstance()->init()))
    {
        // Exiting program if failed initialization
        std::cout << "Failed to initialize Blunder Software, exiting program" << std::endl;
    }
    else
    {
        // Running Software loop on succesful initialization
        Blunder::getInstance()->runLoop();
    }

    // Cleaning up Singleton Instances
    Blunder::destroyInstance();
    EventSystem::destroyInstance();
    ShaderCross::destroyInstance();
    GraphicsDevice::destroyInstance();

    system("pause");
    return 0;
}