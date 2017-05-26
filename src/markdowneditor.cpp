#include "markdowneditor.h"
#include "ui_markdowneditor.h"
#include <QTextEdit>

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


void MarkdownEditor::load()
{
    openFile(mConfig->markdownFile());
}


void MarkdownEditor::newFile()
{
    mUi->textEdit->clear();
}


void MarkdownEditor::openFile(const QString& path)
{
    QFile file(path);

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QByteArray content = file.readAll();
        QTextDocument* doc = mUi->textEdit->document();
        doc->setPlainText(content);
        doc->setDocumentMargin(80);
        doc->setPlainText(content);
    }
}


void MarkdownEditor::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(18);
    mUi->textEdit->setFont(font);

    mHighlighter = new MarkdownEditorHighlighter(mUi->textEdit->document());

    QFile file("mainwindow.h");

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        mUi->textEdit->setPlainText(file.readAll());
    }
}
