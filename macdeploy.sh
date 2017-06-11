#!/usr/bin/env bash

# CHANGEME
CMAKE_PREFIX_PATH=/Users/galuh/Qt/5.9/clang_64


QMAKE_BIN=$CMAKE_PREFIX_PATH/bin/qmake

rm -rf build
mkdir build
cd build
QT_QMAKE_EXECUTABLE=$QMAKE_BIN CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH cmake -DCMAKE_BUILD_TYPE=Release ..
make -j5

cp -r ../3rdparty/MacOS/ src/Pandora.app/Contents/MacOS/

# macdeployqt must be run from qt bin directory so that qrc still works!
# see https://forum.qt.io/topic/59030/solved-macdeployqt-problem/9
#BUILD_DIR=$(pwd)
#cd $CMAKE_PREFIX_PATH/bin/
#./macdeployqt $BUILD_DIR/src/Pandora.app
