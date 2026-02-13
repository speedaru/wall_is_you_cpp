cmake_minimum_required(VERSION 3.12)
project(WallIsYouCpp)

# 1. Set C++20 Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Point to your local SFML build
set(SFML_DIR "$ENV{HOME}/sfml_install/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE) # Tells CMake to prioritize the .a files

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# 3. Collect all source files recursively
# This looks into wall_is_you/ and all subfolders in wall_is_you/src/
file(GLOB_RECURSE SRC_FILES 
    "wall_is_you/main.cpp"
    "wall_is_you/src/*.cpp"
    "wall_is_you/src/*.hpp"
)

# 4. Define the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE "wall_is_you/pch.h")

# 5. Include Directories
# We include the local 'include' folder and the 'src' folders
target_include_directories(${PROJECT_NAME} PRIVATE 
    "wall_is_you/thirdparty/"
    "wall_is_you/include/"
    "wall_is_you/src/"
    "wall_is_you/"
)

# preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    "__PROJECT_DIR__=\"${CMAKE_SOURCE_DIR}/wall_is_you/\""
)

# 6. Link Libraries
# Notice we link against SFML and JSON. 
# 'pthread' is often required on Linux for multi-threading.
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
    GL
    X11
    udev
    pthread
)

# This ensures the 'assets' and 'dungeons' folders are where the .exe is
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/wall_is_you/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/wall_is_you/dungeons $<TARGET_FILE_DIR:${PROJECT_NAME}>/dungeons
)
