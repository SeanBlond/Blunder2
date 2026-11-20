cmake_minimum_required (VERSION 3.28.3)
project ("Blunder2")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Finding the various SDL libraries from vcpkg
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_shadercross CONFIG REQUIRED)

# Creating the executable
add_executable(
    BlunderEngine
    
    # Engine Main
    "Apps/src/EngineMain.cpp"
)

# Custom library for math functions and whatnot
add_subdirectory(SMATH)
target_link_libraries(BlunderEngine PRIVATE SMATH)

# Custom library for the Blunder Engine
add_subdirectory(ENGINE)

# Adding the various libraries to the engine (smath, sdl, etc)
target_link_libraries(ENGINE PRIVATE SMATH)

# Linking the Engine to the Project
target_link_libraries(BlunderEngine PRIVATE ENGINE)

# Adding resources folder to the executable
add_custom_command(TARGET BlunderEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/resources
    $<TARGET_FILE_DIR:BlunderEngine>/resources
)