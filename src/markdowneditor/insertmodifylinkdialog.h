#ifndef INSERTLINKDIALOG_H
#define INSERTLINKDIALOG_H

#include <QDialog>

namespace Ui
{
class InsertLinkDialog;
}

class InsertModifyLinkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertModifyLinkDialog(QWidget* parent = 0);
    ~InsertModifyLinkDialog();

    void setText(const QString& text);
    void setLink(const QString& link);

    QString text();
    QString link();

    void fromMarkdownFormat(const QString& fmt);
    QString toMarkdownFormat();

public slots:
    virtual int exec();

private:
    Ui::InsertLinkDialog* mUi;
};

#endif // INSERTLINKDIALOG_H
