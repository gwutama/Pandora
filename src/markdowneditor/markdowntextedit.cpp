#include "markdowntextedit.h"
#include <QDebug>

const char* MarkdownTextEdit::sTag = "[MarkdownTextEdit]";

MarkdownTextEdit::MarkdownTextEdit(QWidget* parent) :
    QPlainTextEdit(parent)
{
}


MarkdownTextEdit::~MarkdownTextEdit()
{
}


void MarkdownTextEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Tab)
    {
        indent(event);
        return;
    }
    else if (event->key() == Qt::Key_Backtab)
    {
        outdent(event);
        return;
    }

    QPlainTextEdit::keyPressEvent(event);
}


void MarkdownTextEdit::indent(QKeyEvent* event)
{
    QTextCursor currentCursor = textCursor();
    QString currentText = currentCursor.selectedText();

    if (currentText.isEmpty())
    {
        currentCursor.insertText("  " + currentText);
        event->accept();
        return;
    }
    else
    {
        QStringList lines = currentText.split(QChar::ParagraphSeparator);
        QString newText;

        foreach (QString line, lines)
        {
            newText += "  " + line + "\n";
        }

        newText.chop(1);
        currentCursor.insertText(newText);
        event->accept();
        return;
    }
}


void MarkdownTextEdit::outdent(QKeyEvent *event)
{
    QTextCursor currentCursor = textCursor();
    QString currentText = currentCursor.selectedText();

    if (currentText.isEmpty())
    {
        for (int i = 0; i < 2; i++)
        {
            currentCursor.movePosition(QTextCursor::Left);
            QChar c = document()->characterAt(currentCursor.position());

            if (c == ' ')
            {
                currentCursor.deleteChar();
            }
            else
            {
                break;
            }
        }

        event->accept();
        return;
    }
    else
    {
        QStringList lines = currentText.split(QChar::ParagraphSeparator);
        QString newText;

        foreach (QString line, lines)
        {
            for (int i = 0; i < 2; i++)
            {
                if (line.startsWith(' '))
                {
                    line.remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            newText += line + "\n";
        }

        newText.chop(1);
        currentCursor.insertText(newText);
        event->accept();
        return;
    }
}
