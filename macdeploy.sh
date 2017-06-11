#!/usr/bin/env bash

# CHANGEME
export CMAKE_PREFIX_PATH=/Users/galuh/Qt/5.7/clang_64
MACDEPLOYQT_BIN=/Users/galuh/Qt/5.7/clang_64/bin/macdeployqt


rm -rf build
mkdir build
cd build
cmake ..
make -j5
cp -r ../3rdparty/MacOS/ src/Pandora.app/Contents/MacOS/
$MACDEPLOYQT_BIN src/Pandora.app -dmg
