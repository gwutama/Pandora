#include "markdowneditor.h"
#include "ui_markdowneditor.h"
#include <QTextEdit>
#include <QFileInfo>
#include <QDebug>

const char* MarkdownEditor::sTag = "[MarkdownEditor]";

MarkdownEditor::MarkdownEditor(QSharedPointer<AppConfig> config,
                               QWidget* parent) :
    QWidget(parent),
    mConfig(config),
    mUi(new Ui::MarkdownEditor)
{
    mUi->setupUi(this);
    setupEditor();

    connect(&mContentChangeTimer, &QTimer::timeout, this, &MarkdownEditor::checkContentChanged);
}


MarkdownEditor::~MarkdownEditor()
{
    delete mUi;
}


void MarkdownEditor::checkContentChanged()
{
    QString content = mUi->textEdit->document()->toPlainText();

    if (mOldContent != content)
    {
        emit contentChanged(content);
        mOldContent = content;
    }
}


void MarkdownEditor::refocusEditor()
{
    mUi->textEdit->setFocus();
}

bool MarkdownEditor::open()
{
    if (openFile(mConfig->markdownFile()))
    {
        qDebug() << sTag << "Content has been changed";
        mContentChangeTimer.setSingleShot(false);
        mContentChangeTimer.start(3000); // 3 seconds
        checkContentChanged();
        return true;
    }

    return false;
}


void MarkdownEditor::close()
{
    mUi->textEdit->clear();
    mContentChangeTimer.stop();
}


bool MarkdownEditor::save()
{
    return saveAs(mConfig->markdownFile());
}


bool MarkdownEditor::saveAs(const QString& path)
{
    if (path.isEmpty())
    {
        qWarning() << sTag << "Cannot save because file path is empty";
        return false;
    }

    QFile file(path);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qWarning() << sTag << "Cannot open file for writing:" << path;
        return false;
    }

    QString content = mUi->textEdit->document()->toPlainText();
    qDebug() << sTag << "Saving content to file:" << content;

    file.write(content.toUtf8());
    file.close();
    return true;
}


bool MarkdownEditor::openFile(const QString& path)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << sTag << "Cannot open file for reading:" << path;
        return false;
    }

    QByteArray content = file.readAll();
    QTextDocument* doc = mUi->textEdit->document();
    doc->setPlainText(content);
    doc->setDocumentMargin(70);
    doc->setPlainText(content);

    file.close();
    return true;
}


void MarkdownEditor::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(18);
    mUi->textEdit->setFont(font);

    QFontMetrics metrics(font);
    mUi->textEdit->setTabStopWidth(4 * metrics.width(' ')); // replace tab with 4 spaces

    MarkdownEditorHighlighter* mhPtr = new MarkdownEditorHighlighter(mUi->textEdit->document());
    mHighlighter = QSharedPointer<MarkdownEditorHighlighter>(mhPtr);
}
