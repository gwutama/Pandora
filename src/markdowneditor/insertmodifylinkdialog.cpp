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


void InsertModifyLinkDialog::setText(const QString& text)
{
    mUi->textLineEdit->setText(text);
}


void InsertModifyLinkDialog::setLink(const QString& link)
{
    mUi->linkLineEdit->setText(link);
}


QString InsertModifyLinkDialog::text()
{
    return mUi->textLineEdit->text();
}


QString InsertModifyLinkDialog::link()
{
    return mUi->linkLineEdit->text();
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

        QString text = matches.at(1);
        setText(text);

        QString link = matches.at(2);
        setLink(link);
    }
}


QString InsertModifyLinkDialog::toMarkdownFormat()
{
    // Build e.g. [a website](http://foo.bar)
    QString fmt("[%1](%2)");
    fmt = fmt.arg(text()).arg(link());
    return fmt;
}


int InsertModifyLinkDialog::exec()
{
    if (text().isEmpty() && link().isEmpty())
    {
        setWindowTitle("Insert Link");
    }
    else
    {
        setWindowTitle("Modify Link");
    }

    return QDialog::exec();
}
