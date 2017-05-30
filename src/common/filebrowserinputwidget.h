#ifndef FILEBROWSERINPUTWIDGET_H
#define FILEBROWSERINPUTWIDGET_H

#include <QWidget>

namespace Ui
{
class FileBrowserInputWidget;
}

class FileBrowserInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowserInputWidget(QWidget* parent = 0);
    ~FileBrowserInputWidget();

    void setFileName(const QString& fileName);
    QString fileName();

private slots:
    void onBrowsePushButtonClicked(bool checked);

signals:
    void accepted();

private:
    Ui::FileBrowserInputWidget* mUi;
};

#endif // FILEBROWSERINPUTWIDGET_H
