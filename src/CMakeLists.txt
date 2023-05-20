set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    maintoolbar.cpp
    preferences/preferencesdialog.cpp
    common/appconfig.cpp
    common/filebrowserinputwidget.cpp
    markdownviewer/documentgenerator.cpp
    markdownviewer/markdownviewer.cpp
    markdowneditor/markdowneditor.cpp
    markdowneditor/markdowneditorhighlighter.cpp
    markdowneditor/findreplacewidget.cpp
    markdowneditor/insertmodifylinkdialog.cpp
    markdowneditor/insertmodifyimagedialog.cpp
    markdowneditor/imagepreviewdialog.cpp
    markdowneditor/markdowntextedit.cpp
    markdowneditor/documentstatisticsdialog.cpp
    markdowneditor/spellcheck.cpp
    markdowneditor/languagetool.cpp
    markdowneditor/languagetoolmatch.cpp
    markdowneditor/textmanipulationactionsdelegate.cpp
    markdowneditor/findreplaceactionsdelegate.cpp
    markdowneditor/spellcheckactionsdelegate.cpp
    markdowneditor/grammarcheckactionsdelegate.cpp
    markdowneditor/proofreadactionsdelegatebase.cpp
    markdowncollection/markdowncollection.cpp
    markdowncollection/markdowncollectionitem.cpp
    markdowncollection/collectionlistview.cpp
    markdowncollection/richtextstyleditemdelegate.cpp

    mainwindow.ui
    maintoolbar.ui
    preferences/preferencesdialog.ui
    common/filebrowserinputwidget.ui
    markdowneditor/markdowneditor.ui
    markdowneditor/findreplacewidget.ui
    markdowneditor/insertmodifylinkdialog.ui
    markdowneditor/insertmodifyimagedialog.ui
    markdowneditor/imagepreviewdialog.ui
    markdowneditor/documentstatisticsdialog.ui

    resources.qrc
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets Network Core5Compat)

set(HUNSPELL_INCLUDE_DIR "/opt/homebrew/Cellar/hunspell/1.7.2/include/")
set(HUNSPELL_LIBRARIES "/opt/homebrew/Cellar/hunspell/1.7.2/lib/libhunspell-1.7.a")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${HUNSPELL_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE
                      Qt6::Core Qt6::Gui Qt6::Widgets Qt6::WebEngineWidgets Qt6::Network Qt6::Core5Compat
                      ${HUNSPELL_LIBRARIES})

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
