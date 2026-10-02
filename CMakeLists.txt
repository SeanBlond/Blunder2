cmake_minimum_required (VERSION 3.28.3)
project ("Blunder2")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Creating the executable
add_executable(${PROJECT_NAME} 
    # main
    "src/main.cpp"

    # icon resources
    "resources/bl2Resources.rc"

    # Color
    "src/color/color.h"
    "src/color/color.cpp"

    # Graphics
    "src/graphics/ShaderCross.h" "src/graphics/ShaderCross.cpp"
    "src/graphics/GraphicsDevice.h" "src/graphics/GraphicsDevice.cpp"
    "src/graphics/GraphicsRenderer.h" "src/graphics/GraphicsRenderer.cpp"

    # Graphics - Buffers
    "src/graphics/Buffers/Vertices.h"   
    "src/graphics/Buffers/PosColorBuffer.h" "src/graphics/Buffers/PosColorBuffer.cpp"
    "src/graphics/Buffers/UITextureBuffer.h" "src/graphics/Buffers/UITextureBuffer.cpp"
    "src/graphics/Buffers/TextBuffer.h" "src/graphics/Buffers/TextBuffer.cpp" 
    
    # UI
    "src/UI/UI.h"

    # UI - Basics
    "src/UI/UIElement.h"
    "src/UI/UIPositioning.h"
    "src/UI/UIRenderer.h"
    "src/UI/UIWindows.h" "src/UI/UIWindows.cpp"

    # UI - Windows
    "src/UI/Windows/TestWindow.h" "src/UI/Windows/TestWindow.cpp"
    "src/UI/Windows/ComponentWindow.h" "src/UI/Windows/ComponentWindow.cpp"
    
    # UI - Elements
    "src/UI/Elements/FloatSlider.h" "src/UI/Elements/FloatSlider.cpp"
    "src/UI/Elements/Text.h" "src/UI/Elements/Text.cpp" 
    "src/UI/Elements/ImageButton.h" "src/UI/Elements/ImageButton.cpp"
    "src/UI/Elements/ImageToggle.h" "src/UI/Elements/ImageToggle.cpp"
    "src/UI/Elements/TextButton.h" "src/UI/Elements/TextButton.cpp"
    "src/UI/Elements/Dropdown.h" "src/UI/Elements/Dropdown.cpp"     
    "src/UI/Elements/InvisibleButton.h" "src/UI/Elements/InvisibleButton.cpp"

    # Runtime
    "src/Runtime/WindowManagement.h" "src/Runtime/WindowManagement.cpp" 
    "src/Runtime/LockedWindow.h" "src/Runtime/LockedWindow.cpp" 
    "src/Runtime/Time.h" "src/Runtime/Time.cpp"

    # Runtime - Project Stuff
    "src/Runtime/Project/ProjectManager.h" "src/Runtime/Project/ProjectManager.cpp"
    "src/Runtime/Project/RecentProjectManager.h" "src/Runtime/Project/RecentProjectManager.cpp"
    
    # Event System
    "src/Events/Event.h" 
    "src/Events/EventSystem.h" "src/Events/EventSystem.cpp"
    "src/Events/EventListener.h" "src/Events/EventListener.cpp" 

    # Input System
    "src/Input/Input.h" "src/Input/Input.cpp"
    "src/Input/KeyMap.h" "src/Input/KeyMap.cpp"

    # GameObjects
    "src/GameObject/GameObject.h" "src/GameObject/GameObject.cpp" 

    # Blunder (Software Class)
    "src/Runtime/Blunder.h" "src/Runtime/Blunder.cpp"
 "src/UI/TextInput.h" "src/UI/TextInput.cpp" "src/UI/Elements/FloatEntry.h" "src/UI/Elements/FloatEntry.cpp")

# Custom library for math functions and whatnot
add_library(
    SMATH STATIC

    # Math - Functions
    "smath/src/clamping.h"
    "smath/src/conversions.h"
    "smath/src/trig.h"

    # Math - Vectors
    "smath/src/vectors.h" "smath/src/vectors.cpp"
    "smath/src/ivectors.h" "smath/src/ivectors.cpp"

    # Math - Matrices
    "smath/src/mat4.h" "smath/src/mat4.cpp"

    # Math - Tansforms
    "smath/src/transform.h"
)
target_include_directories(SMATH PUBLIC smath/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SMATH)

# SDL related libraries installed via vcpkg
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

find_package(SDL3_image CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)

find_package(SDL3_ttf CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf)

find_package(SDL3_shadercross CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_shadercross::SDL3_shadercross-shared)

# Adding resources folder to the executable
add_custom_command(TARGET Blunder2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:Blunder2>/resources
)