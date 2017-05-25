#include "filebrowserinputwidget.h"
#include "ui_filebrowserinputwidget.h"
#include <QFileDialog>

FileBrowserInputWidget::FileBrowserInputWidget(QWidget* parent) :
    QWidget(parent),
    mUi(new Ui::FileBrowserInputWidget)
{
    mUi->setupUi(this);
    connect(mUi->browsePushButton, &QPushButton::clicked,
            this, &FileBrowserInputWidget::onBrowsePushButtonClicked);    
}


FileBrowserInputWidget::~FileBrowserInputWidget()
{
    delete mUi;
}


void FileBrowserInputWidget::onBrowsePushButtonClicked(bool /*checked*/)
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);

    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();

        if (fileNames.size()) // user selected at least a file
        {
            QString fileName = fileNames.at(0);
            setFileName(fileName);
        }
    }
}


void FileBrowserInputWidget::setFileName(const QString& fileName)
{
    mUi->pathLineEdit->setText(fileName);
}


QString FileBrowserInputWidget::fileName()
{
    return mUi->pathLineEdit->text();
}
