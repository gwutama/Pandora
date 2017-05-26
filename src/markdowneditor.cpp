#include "markdowneditor.h"
#include "ui_markdowneditor.h"
#include <QTextEdit>
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
}


MarkdownEditor::~MarkdownEditor()
{
    delete mUi;
}


void MarkdownEditor::open()
{
    openFile(mConfig->markdownFile());
}


void MarkdownEditor::close()
{
    mUi->textEdit->clear();
}


void MarkdownEditor::save()
{
    if (mConfig->markdownFile().isEmpty())
    {
        qWarning() << sTag << "Cannot save because markdown file is not loaded";
        return;
    }

    QFile file(mConfig->markdownFile());

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qWarning() << sTag << "Cannot open markdown file for writing:" << mConfig->markdownFile();
        return;
    }

    QString contents = mUi->textEdit->document()->toPlainText();
    qDebug() << sTag << "Saving contents to file:" << contents;

    file.write(contents.toUtf8());
    file.close();
}


void MarkdownEditor::openFile(const QString& path)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << sTag << "Cannot open markdown file for reading:" << mConfig->markdownFile();
        return;
    }

    QByteArray content = file.readAll();
    QTextDocument* doc = mUi->textEdit->document();
    doc->setPlainText(content);
    doc->setDocumentMargin(80);
    doc->setPlainText(content);

    file.close();
}


void MarkdownEditor::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(18);
    mUi->textEdit->setFont(font);

    MarkdownEditorHighlighter* mhPtr = new MarkdownEditorHighlighter(mUi->textEdit->document());
    mHighlighter = QSharedPointer<MarkdownEditorHighlighter>(mhPtr);
}
