#include "formattingtoolbar.h"
#include "ui_formattingtoolbar.h"

FormattingToolbar::FormattingToolbar(QWidget* parent) :
    QWidget(parent),
    mUi(new Ui::FormattingToolbar)
{
    mUi->setupUi(this);

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
