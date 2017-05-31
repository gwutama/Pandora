#ifndef IMAGEPREVIEWDIALOG_H
#define IMAGEPREVIEWDIALOG_H

#include <QDialog>

namespace Ui
{
class ImagePreviewDialog;
}

class ImagePreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImagePreviewDialog(QWidget* parent = 0);
    ~ImagePreviewDialog();

public slots:
    bool previewImage(const QString& path);

private:
    Ui::ImagePreviewDialog* mUi;
    static const char* sTag;
};

#endif // IMAGEPREVIEWDIALOG_H
