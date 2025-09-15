#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status.
set -e

# Debug export
mkdir -p ./build/debug
pushd ./build/debug
    cmake -G 'Ninja' -DCMAKE_BUILD_TYPE=Debug  ../.. && \
    ninja
    ret=$?
popd

# Release export
mkdir -p ./build/release
pushd ./build/release
    cmake -G 'Ninja' -DCMAKE_BUILD_TYPE=Release  ../.. && \
    ninja
    ret=$?
popd

# To release the game into the public (make sure to also include the CMakeLists change for this):
# install_name_tool -rpath {RELEASE DIRECTORY} @executable_path ./{EXECUTABLE NAME}
# ex: install_name_tool -rpath /Users/davidjerzak/C++/SDL-VSCode/build/release/Release @executable_path ./SDL-VSCODE

# For better understanding of this, visit: https://wiki.avblocks.com/avblocks-for-cpp/getting-started-macos/create-cpp-command-line-tool-cmake/