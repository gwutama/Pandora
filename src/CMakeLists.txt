find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    appconfig.cpp
    processrunner.cpp
    preferencesdialog.cpp
    filebrowserinputwidget.cpp
    documentgenerator.cpp
)

qt5_add_resources(RESOURCE_FILES resources.qrc)

qt5_wrap_ui(UI_FILES
    mainwindow.ui
    preferencesdialog.ui 
    filebrowserinputwidget.ui
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# easyloggingpp definitions
add_definitions(-DELPP_UNICODE
                -DELPP_THREAD_SAFE
                -DELPP_FORCE_USE_STD_THREAD
                -DELPP_STACKTRACE_ON_CRASH
                -DELPP_NO_DEFAULT_LOG_FILE
                -DELPP_QT_LOGGING)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/easyloggingpp/src/)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${UI_FILES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebEngineWidgets)

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST MacOSXBundleInfo.plist.in
        MACOSX_BUNDLE_INFO_STRING "Markdown editor made for pandoc"
        MACOSX_BUNDLE_LONG_VERSION_STRING 0.1
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_SHORT_VERSION_STRING 0.1
        MACOSX_BUNDLE_BUNDLE_VERSION 0.1
        MACOSX_BUNDLE_COPYRIGHT "Copyright 2017 Galuh Utama"
    )
endif()
