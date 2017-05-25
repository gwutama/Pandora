#include "markdowneditor.h"
#include "ui_markdowneditor.h"

MarkdownEditor::MarkdownEditor(QSharedPointer<AppConfig> config,
                               QWidget* parent) :
    QWidget(parent),
    mConfig(config),
    ui(new Ui::MarkdownEditor)
{
    ui->setupUi(this);
    setupEditor();
}


MarkdownEditor::~MarkdownEditor()
{
    delete ui;
}


void MarkdownEditor::load()
{
    openFile(mConfig->markdownFile());
}


void MarkdownEditor::newFile()
{
    ui->textEdit->clear();
}


void MarkdownEditor::openFile(const QString& path)
{
    QFile file(path);

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QByteArray read = file.readAll();
        ui->textEdit->setText(read);
    }
}


void MarkdownEditor::setupEditor()
{
    mHighlighter = new MarkdownEditorHighlighter(ui->textEdit->document());

    QFile file("mainwindow.h");

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->textEdit->setPlainText(file.readAll());
    }
}

