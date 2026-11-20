cmake_minimum_required (VERSION 3.28.3)
project ("Blunder2")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Creating the executable
add_executable(${PROJECT_NAME} 
    # main
    "Engine/main.cpp"

    # icon resources
    "Engine/resources/bl2Resources.rc"

    # Color
    "Engine/color/color.h"
    "Engine/color/color.cpp"

    # Graphics
    "Engine/graphics/ShaderCross.h" "Engine/graphics/ShaderCross.cpp"
    "Engine/graphics/GraphicsDevice.h" "Engine/graphics/GraphicsDevice.cpp"
    "Engine/graphics/GraphicsRenderer.h" "Engine/graphics/GraphicsRenderer.cpp"

    # Graphics - Buffers
    "Engine/graphics/Buffers/Vertices.h"   
    "Engine/graphics/Buffers/PosColorBuffer.h" "Engine/graphics/Buffers/PosColorBuffer.cpp"
    "Engine/graphics/Buffers/UITextureBuffer.h" "Engine/graphics/Buffers/UITextureBuffer.cpp"
    "Engine/graphics/Buffers/TextBuffer.h" "Engine/graphics/Buffers/TextBuffer.cpp" 
    "Engine/graphics/Buffers/GameObjectBuffer.h" "Engine/graphics/Buffers/GameObjectBuffer.cpp"
    
    # UI
    "Engine/UI/UI.h"

    # UI - Basics
    "Engine/UI/UIElement.h"
    "Engine/UI/UIPositioning.h"
    "Engine/UI/UIRenderer.h"
    "Engine/UI/UIWindows.h" "Engine/UI/UIWindows.cpp"
     "Engine/UI/TextInput.h" "Engine/UI/TextInput.cpp"

    # UI - Windows
    "Engine/UI/Windows/TestWindow.h" "Engine/UI/Windows/TestWindow.cpp"
    "Engine/UI/Windows/ComponentWindow.h" "Engine/UI/Windows/ComponentWindow.cpp"
    "Engine/UI/Windows/ViewportWindow.h" "Engine/UI/Windows/ViewportWindow.cpp"
    "Engine/UI/Windows/HierarchyWindow.h" "Engine/UI/Windows/HierarchyWindow.cpp"
    "Engine/UI/Windows/MaterialWindow.h" "Engine/UI/Windows/MaterialWindow.cpp" 

    # UI - Pop-Up Windows
    # "Engine/UI/Windows/Pop-Up Windows/ColorWindow.h" "Engine/UI/Windows/Pop-Up Windows/ColorWindow.cpp"
    
    # UI - Elements
    "Engine/UI/Elements/HierarchyTextEntry.h" "Engine/UI/Elements/HierarchyTextEntry.cpp"
    "Engine/UI/Elements/ImageButton.h" "Engine/UI/Elements/ImageButton.cpp"
    "Engine/UI/Elements/ImageToggle.h" "Engine/UI/Elements/ImageToggle.cpp"
    "Engine/UI/Elements/TextButton.h" "Engine/UI/Elements/TextButton.cpp"
    "Engine/UI/Elements/Dropdown.h" "Engine/UI/Elements/Dropdown.cpp"     
    "Engine/UI/Elements/ViewportScroller.h" "Engine/UI/Elements/ViewportScroller.cpp"     
    "Engine/UI/Elements/InvisibleButton.h" "Engine/UI/Elements/InvisibleButton.cpp"
    "Engine/UI/Elements/TextEntry.h" "Engine/UI/Elements/TextEntry.cpp"
    "Engine/UI/Elements/UniformTextEntry.h" "Engine/UI/Elements/UniformTextEntry.cpp"

    # UI - Labelled Elements
    "Engine/UI/Elements/Labelled Elements/IntEntry.h" "Engine/UI/Elements/Labelled Elements/IntEntry.cpp"
    "Engine/UI/Elements/Labelled Elements/FloatEntry.h" "Engine/UI/Elements/Labelled Elements/FloatEntry.cpp"
    "Engine/UI/Elements/Labelled Elements/Float2Entry.h" "Engine/UI/Elements/Labelled Elements/Float2Entry.cpp"
    "Engine/UI/Elements/Labelled Elements/Float3Entry.h" "Engine/UI/Elements/Labelled Elements/Float3Entry.cpp"
    "Engine/UI/Elements/Labelled Elements/Float4Entry.h" "Engine/UI/Elements/Labelled Elements/Float4Entry.cpp"
    "Engine/UI/Elements/Labelled Elements/ColorEntry.h" "Engine/UI/Elements/Labelled Elements/ColorEntry.cpp"
    "Engine/UI/Elements/Labelled Elements/MaterialDropdown.h" "Engine/UI/Elements/Labelled Elements/MaterialDropdown.cpp" 
    "Engine/UI/Elements/Labelled Elements/LabelledTextEntry.h" "Engine/UI/Elements/Labelled Elements/LabelledTextEntry.cpp"
    "Engine/UI/Elements/Labelled Elements/LabelledToggle.h" "Engine/UI/Elements/Labelled Elements/LabelledToggle.cpp"

    # Runtime
    "Engine/Runtime/WindowManagement.h" "Engine/Runtime/WindowManagement.cpp" 
    "Engine/Runtime/LockedWindow.h" "Engine/Runtime/LockedWindow.cpp" 
    "Engine/Runtime/Time.h" "Engine/Runtime/Time.cpp"

    # Runtime - Project Stuff
    "Engine/Runtime/Project/ProjectManager.h" "Engine/Runtime/Project/ProjectManager.cpp"
    "Engine/Runtime/Project/RecentProjectManager.h" "Engine/Runtime/Project/RecentProjectManager.cpp"
    
    # Event System
    "Engine/Events/Event.h" 
    "Engine/Events/EventSystem.h" "Engine/Events/EventSystem.cpp"
    "Engine/Events/EventListener.h" "Engine/Events/EventListener.cpp" 

    # Input System
    "Engine/Input/Input.h" "Engine/Input/Input.cpp"
    "Engine/Input/KeyMap.h" "Engine/Input/KeyMap.cpp"

    # GameObjects
    "Engine/GameObject/GameObject.h" "Engine/GameObject/GameObject.cpp" 
    "Engine/GameObject/GameObjectManager.h" "Engine/GameObject/GameObjectManager.cpp"
    "Engine/GameObject/Camera.h" "Engine/GameObject/Camera.cpp" 
    
    # Material System
    "Engine/Material/Uniforms.h" "Engine/Material/Uniform.cpp"
    "Engine/Material/Material.h" "Engine/Material/Material.cpp"

    # Resource Management
    "Engine/ResourceManagement/ResourceManager.h" "Engine/ResourceManagement/ResourceManager.cpp"
    "Engine/ResourceManagement/MaterialManager.h" "Engine/ResourceManagement/MaterialManager.cpp" 

    # Blunder (Software Class)
    "Engine/Runtime/Blunder.h" "Engine/Runtime/Blunder.cpp"

)

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
    "smath/src/transform.h" "smath/src/transform2D.h"
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
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/resources
    $<TARGET_FILE_DIR:Blunder2>/resources
)