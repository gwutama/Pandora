#include "formattingtoolbar.h"
#include "ui_formattingtoolbar.h"

FormattingToolbar::FormattingToolbar(QWidget* parent) :
    QWidget(parent),
    mUi(new Ui::FormattingToolbar)
{
    mUi->setupUi(this);

    // Document
    connect(mUi->newDocumentButton, &QPushButton::clicked,
            this, &FormattingToolbar::newDocumentButtonClicked);
    connect(mUi->saveDocumentButton, &QPushButton::clicked,
            this, &FormattingToolbar::saveDocumentButtonClicked);

    // Undo / redo
    connect(mUi->undoButton, &QPushButton::clicked,
            this, &FormattingToolbar::undoButtonClicked);
    connect(mUi->redoButton, &QPushButton::clicked,
            this, &FormattingToolbar::redoButtonClicked);

    // Formatting
    connect(mUi->boldSelectionButton, &QPushButton::clicked,
            this, &FormattingToolbar::boldSelectionButtonClicked);
    connect(mUi->italicSelectionButton, &QPushButton::clicked,
            this, &FormattingToolbar::italicSelectionButtonClicked);
    connect(mUi->strikeoutSelectionButton, &QPushButton::clicked,
            this, &FormattingToolbar::strikeoutSelectionButtonClicked);
}


FormattingToolbar::~FormattingToolbar()
{
    delete mUi;
}
