#include "formattingtoolbar.h"
#include "ui_formattingtoolbar.h"

FormattingToolbar::FormattingToolbar(QWidget* parent) :
    QWidget(parent),
    mUi(new Ui::FormattingToolbar)
{
    mUi->setupUi(this);

    connect(mUi->boldSelectionButton, &QToolButton::clicked,
            this, &FormattingToolbar::boldSelectionButtonClicked);
    connect(mUi->italicSelectionButton, &QToolButton::clicked,
            this, &FormattingToolbar::italicSelectionButtonClicked);
    connect(mUi->strikeoutSelectionButton, &QToolButton::clicked,
            this, &FormattingToolbar::strikeoutSelectionButtonClicked);
}


FormattingToolbar::~FormattingToolbar()
{
    delete mUi;
}
