#include "markdowneditor.h"
#include "ui_markdowneditor.h"
#include <QFileDialog>

MarkdownEditor::MarkdownEditor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MarkdownEditor)
{
    ui->setupUi(this);
    setupEditor();
}


MarkdownEditor::~MarkdownEditor()
{
    delete ui;
}


void MarkdownEditor::newFile()
{
    ui->textEdit->clear();
}


void MarkdownEditor::openFile(const QString& path)
{
    QString fileName = path;

    if (fileName.isNull())
    {
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                "Markdown Files (*.md *.txt)");
    }

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            ui->textEdit->setPlainText(file.readAll());
        }
    }
}


void MarkdownEditor::setupEditor()
{
    QFont font;
    font.setFamily("Monaco");
    font.setFixedPitch(true);
    font.setPointSize(14);

    ui->textEdit->setFont(font);

    //highlighter = new Highlighter(editor->document());

    QFile file("mainwindow.h");

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->textEdit->setPlainText(file.readAll());
    }
}

