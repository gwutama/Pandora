#include "maintoolbar.h"
#include "ui_maintoolbar.h"

MainToolbar::MainToolbar(QWidget* parent) :
    QWidget(parent),
    mUi(new Ui::FormattingToolbar)
{
    mUi->setupUi(this);

    // Document
    connect(mUi->newDocumentButton, &QPushButton::clicked,
            this, &MainToolbar::newDocumentButtonClicked);
    connect(mUi->saveDocumentButton, &QPushButton::clicked,
            this, &MainToolbar::saveDocumentButtonClicked);

    // Undo / redo
    connect(mUi->undoButton, &QPushButton::clicked, this, &MainToolbar::undoButtonClicked);
    connect(mUi->redoButton, &QPushButton::clicked, this, &MainToolbar::redoButtonClicked);

    // Formatting
    connect(mUi->boldSelectionButton, &QPushButton::clicked,
            this, &MainToolbar::boldSelectionButtonClicked);
    connect(mUi->italicSelectionButton, &QPushButton::clicked,
            this, &MainToolbar::italicSelectionButtonClicked);
    connect(mUi->strikeoutSelectionButton, &QPushButton::clicked,
            this, &MainToolbar::strikeoutSelectionButtonClicked);

    // Others
    connect(mUi->linkButton, &QPushButton::clicked, this, &MainToolbar::linkButtonClicked);
    connect(mUi->imageButton, &QPushButton::clicked, this, &MainToolbar::imageButtonClicked);
    connect(mUi->horizontalLineButton, &QPushButton::clicked,
            this, &MainToolbar::horizontalLineButtonClicked);
}


MainToolbar::~MainToolbar()
{
    delete mUi;
}
