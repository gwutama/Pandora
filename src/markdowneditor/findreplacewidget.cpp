#include "findreplacewidget.h"
#include "ui_findreplacewidget.h"

FindReplaceWidget::FindReplaceWidget(QWidget* parent) :
    QWidget(parent),
    mUi(new Ui::FindReplaceWidget)
{
    mUi->setupUi(this);

    connect(mUi->findLineEdit, &QLineEdit::returnPressed,
            this, &FindReplaceWidget::onReturnPressed);

    connect(mUi->findNextButton, &QToolButton::clicked,
            this, &FindReplaceWidget::nextButtonClicked);
    connect(mUi->findPreviousButton, &QToolButton::clicked,
            this, &FindReplaceWidget::previousButtonClicked);
}


FindReplaceWidget::~FindReplaceWidget()
{
    delete mUi;
}


void FindReplaceWidget::onReturnPressed()
{
    emit textChanged(mUi->findLineEdit->text());
}


void FindReplaceWidget::setFocus()
{
    mUi->findLineEdit->setFocus();
    mUi->findLineEdit->selectAll();
}


void FindReplaceWidget::setFoundNumber(int found)
{
    QString text("%1 found");
    text = text.arg(found);
    mUi->foundLabel->setText(text);
}
