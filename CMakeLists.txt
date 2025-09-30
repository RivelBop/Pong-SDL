cmake_minimum_required(VERSION 3.16)
project(PONG-SDL)

# brew install cmake
# brew install ninja

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# This assumes the SDL_image source is available in vendored/SDL_image
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

# This assumes the SDL_mixer source is available in vendored/SDL_mixer
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)

# Create your game executable target as usual
# Add files to executable here
add_executable(PONG-SDL 
    src/main.cpp
    src/pong.cpp
)

# To release the game to the public (make sure to also include the build.sh run command change for this):
# Set the RPATH to be the same directory as the executable
#set_target_properties(SDL-VSCODE PROPERTIES
#    MACOSX_RPATH TRUE
#    CMAKE_INSTALL_RPATH "@executable_path"
#)

# Link to the actual SDL3 library.
target_link_libraries(PONG-SDL PRIVATE SDL3_mixer::SDL3_mixer SDL3_image::SDL3_image SDL3::SDL3)