#include "insertmodifylinkdialog.h"
#include "ui_insertmodifylinkdialog.h"
#include <QRegExp>

InsertModifyLinkDialog::InsertModifyLinkDialog(QWidget* parent) :
    QDialog(parent),
    mUi(new Ui::InsertLinkDialog)
{
    mUi->setupUi(this);
}


InsertModifyLinkDialog::~InsertModifyLinkDialog()
{
    delete mUi;
}


void InsertModifyLinkDialog::setAnchor(const QString& anchor)
{
    mUi->anchorEdit->setText(anchor);
}


void InsertModifyLinkDialog::setLink(const QString& link)
{
    mUi->linkEdit->setText(link);
}


QString InsertModifyLinkDialog::anchor()
{
    return mUi->anchorEdit->text();
}


QString InsertModifyLinkDialog::link()
{
    return mUi->linkEdit->text();
}


void InsertModifyLinkDialog::fromMarkdownFormat(const QString& fmt)
{
    QRegExp rx("\\s*\\[(.*)\\]\\((.*)\\)\\s*");

    if (rx.indexIn(fmt) != -1) // we have matches
    {
        QStringList matches = rx.capturedTexts();

        if (matches.size() != 3)
        {
            return;
        }

        QString anchor = matches.at(1);
        setAnchor(anchor);

        QString link = matches.at(2);
        setLink(link);
    }
}


QString InsertModifyLinkDialog::toMarkdownFormat()
{
    // Build e.g. [a website](http://foo.bar)
    QString fmt("[%1](%2)");
    fmt = fmt.arg(anchor()).arg(link());
    return fmt;
}


int InsertModifyLinkDialog::exec()
{
    if (anchor().isEmpty() && link().isEmpty())
    {
        setWindowTitle("Insert Link");
    }
    else
    {
        setWindowTitle("Modify Link");
    }

    return QDialog::exec();
}
