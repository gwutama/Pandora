#ifndef INSERTMODIFYIMAGEDIALOG_H
#define INSERTMODIFYIMAGEDIALOG_H

#include <QDialog>

namespace Ui
{
class InsertModifyImageDialog;
}

class InsertModifyImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertModifyImageDialog(QWidget* parent = 0);
    virtual ~InsertModifyImageDialog();

    void setAnchor(const QString& anchor);
    void setDescription(const QString& description);
    void setFileName(const QString& fileName);

    QString anchor();
    QString description();
    QString fileName();

    void fromMarkdownFormat(const QString& fmt);
    QString toMarkdownFormat();

public slots:
    virtual int exec();

private:
    Ui::InsertModifyImageDialog* mUi;
};

#endif // INSERTMODIFYIMAGEDIALOG_H
