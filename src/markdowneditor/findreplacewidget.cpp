#include "findreplacewidget.h"
#include "ui_findreplacewidget.h"

FindReplaceWidget::FindReplaceWidget(QWidget* parent) :
    QWidget(parent),
    mUi(new Ui::FindReplaceWidget)
{
    mUi->setupUi(this);

    connect(mUi->findLineEdit, &QLineEdit::returnPressed,
            this, &FindReplaceWidget::wantToExecuteSearch);
    connect(mUi->findLineEdit, &QLineEdit::textChanged,
            this, &FindReplaceWidget::searchTermChanged);

    connect(mUi->findNextButton, &QToolButton::clicked,
            this, &FindReplaceWidget::nextButtonClicked);
    connect(mUi->findPreviousButton, &QToolButton::clicked,
            this, &FindReplaceWidget::previousButtonClicked);

    connect(mUi->replaceButton, &QToolButton::clicked,
            this, &FindReplaceWidget::replaceButtonClicked);
    connect(mUi->replaceFindButton, &QToolButton::clicked,
            this, &FindReplaceWidget::replaceFindButtonClicked);
    connect(mUi->replaceAllButton, &QToolButton::clicked,
            this, &FindReplaceWidget::replaceAllButtonClicked);
}


FindReplaceWidget::~FindReplaceWidget()
{
    delete mUi;
}


QString FindReplaceWidget::searchTerm()
{
    return mUi->findLineEdit->text();
}


QString FindReplaceWidget::replacementText()
{
    return mUi->replaceLineEdit->text();
}


void FindReplaceWidget::setFocus()
{
    QWidget::setFocus();
    mUi->findLineEdit->setFocus();
    mUi->findLineEdit->selectAll();
}


void FindReplaceWidget::setNumMatches(int numMatches)
{
    QString text("%1 found");
    text = text.arg(numMatches);
    mUi->foundLabel->setText(text);
    mNumMatches = numMatches;
}
