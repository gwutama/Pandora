#include "findreplacewidget.h"
#include "ui_findreplacewidget.h"

FindReplaceWidget::FindReplaceWidget(QWidget* parent) :
    QWidget(parent),
    mUi(new Ui::FindReplaceWidget)
{
    mUi->setupUi(this);
}


FindReplaceWidget::~FindReplaceWidget()
{
    delete mUi;
}


void FindReplaceWidget::setFocus()
{
    mUi->findLineEdit->setFocus();
}
