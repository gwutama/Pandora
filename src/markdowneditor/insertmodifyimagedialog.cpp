#include "insertmodifyimagedialog.h"
#include "ui_insertmodifyimagedialog.h"
#include <QFileInfo>
#include <QRegExp>


InsertModifyImageDialog::InsertModifyImageDialog(QWidget* parent) :
    QDialog(parent),
    mUi(new Ui::InsertModifyImageDialog)
{
    mUi->setupUi(this);
}


InsertModifyImageDialog::~InsertModifyImageDialog()
{
    delete mUi;
}


void InsertModifyImageDialog::setAnchor(const QString& anchor)
{
    mUi->anchorEdit->setText(anchor);
}


void InsertModifyImageDialog::setDescription(const QString &description)
{
    mUi->descriptionEdit->setText(description);
}


void InsertModifyImageDialog::setFileName(const QString &fileName)
{
    QFileInfo finfo(fileName);

    if (finfo.exists() && finfo.isFile() && finfo.isReadable())
    {
        mUi->imageBrowser->setFileName(fileName);
    }
}


QString InsertModifyImageDialog::anchor()
{
    return mUi->anchorEdit->text();
}


QString InsertModifyImageDialog::description()
{
    return mUi->descriptionEdit->text();
}


QString InsertModifyImageDialog::fileName()
{
    return mUi->imageBrowser->fileName();
}


void InsertModifyImageDialog::fromMarkdownFormat(const QString &fmt)
{
    QRegExp rx("\\s*\\!\\[(.*)\\]\\((.*) \"(.*)\"\\)\\s*");

    if (rx.indexIn(fmt) != -1) // we have matches
    {
        QStringList matches = rx.capturedTexts();

        if (matches.size() != 4)
        {
            return;
        }

        QString anchor = matches.at(1);
        setAnchor(anchor);

        QString fileName = matches.at(2);
        setFileName(fileName);

        QString description = matches.at(3);
        setDescription(description);
    }
}


QString InsertModifyImageDialog::toMarkdownFormat()
{
    // Build e.g. ![example image](example-image.jpg "An exemplary image")
    QString fmt("![%1](%2 \"%3\")");
    fmt = fmt.arg(anchor()).arg(fileName()).arg(description());
    return fmt;
}


int InsertModifyImageDialog::exec()
{
    if (anchor().isEmpty() && description().isEmpty() && fileName().isEmpty())
    {
        setWindowTitle("Insert Image");
    }
    else
    {
        setWindowTitle("Modify Image");
    }

    return QDialog::exec();
}
