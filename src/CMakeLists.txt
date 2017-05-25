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

SET(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${UI_FILES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebEngineWidgets)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
