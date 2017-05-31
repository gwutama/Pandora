#include "imagepreviewdialog.h"
#include "ui_imagepreviewdialog.h"
#include <QFileInfo>
#include <QDebug>

const char* ImagePreviewDialog::sTag = "[ImagePreviewDialog]";

ImagePreviewDialog::ImagePreviewDialog(QWidget* parent) :
    QDialog(parent),
    mUi(new Ui::ImagePreviewDialog)
{
    mUi->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}


ImagePreviewDialog::~ImagePreviewDialog()
{
    delete mUi;
}


bool ImagePreviewDialog::previewImage(const QString& path)
{
    QFileInfo finfo(path);

    if (!finfo.exists() || !finfo.isFile() | !finfo.isReadable())
    {
        qDebug() << sTag << "Image file does not exist:" << path;
        return false;
    }

    QImage img(path);
    QImage thumb = img.scaled(QSize(300, 300), Qt::KeepAspectRatio);
    mUi->label->setPixmap(QPixmap::fromImage(thumb));

    setFixedSize(thumb.size());

    QPoint mousePos = QCursor::pos() + QPoint(20, 20);
    move(mousePos);
    show();

    return true;
}
