find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    common/appconfig.cpp
    preferences/preferencesdialog.cpp
    preferences/filebrowserinputwidget.cpp
    markdownviewer/documentgenerator.cpp
    markdownviewer/markdownviewer.cpp
    markdowneditor/markdowneditor.cpp
    markdowneditor/markdowneditorhighlighter.cpp
    markdowneditor/findreplacewidget.cpp
)

qt5_add_resources(RESOURCE_FILES resources.qrc)

qt5_wrap_ui(UI_FILES
    mainwindow.ui
    preferences/preferencesdialog.ui 
    preferences/filebrowserinputwidget.ui
    markdowneditor/markdowneditor.ui
    markdowneditor/findreplacewidget.ui
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
