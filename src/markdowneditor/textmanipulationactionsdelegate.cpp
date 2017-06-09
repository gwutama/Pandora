#include "textmanipulationactionsdelegate.h"
#include "insertmodifylinkdialog.h"
#include "insertmodifyimagedialog.h"

TextManipulationActionsDelegate::TextManipulationActionsDelegate(MarkdownTextEdit* textEdit,
                                                                 QObject* parent) :
    QObject(parent),
    mTextEdit(textEdit)
{
}


TextManipulationActionsDelegate::~TextManipulationActionsDelegate()
{
}


void TextManipulationActionsDelegate::undo()
{
    mTextEdit->undo();
}


void TextManipulationActionsDelegate::redo()
{
    mTextEdit->redo();
}


void TextManipulationActionsDelegate::toggleSelectionBold()
{
    QTextCursor currentCursor = mTextEdit->textCursor();
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;

    QRegExp boldAsteriskRx("\\*{2}.*\\*{2}");
    QRegExp boldUnderlineRx("_{2}.*_{2}");

    if (text.indexOf(boldAsteriskRx) > -1 || text.indexOf(boldUnderlineRx) > -1)
    {
        newText = text.mid(2, text.size() - 4);
    }
    else
    {
        newText = "**" + text + "**";
    }

    currentCursor.insertText(newText);
}


void TextManipulationActionsDelegate::toggleSelectionItalic()
{
    QTextCursor currentCursor = mTextEdit->textCursor();
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;

    QRegExp italicAsteriskRx("\\*.*\\*");
    QRegExp italicUnderlineRx("_.*_");

    if (text.indexOf(italicAsteriskRx) > -1 || text.indexOf(italicUnderlineRx) > -1)
    {
        newText = text.mid(1, text.size() - 2);
    }
    else
    {
        newText = "*" + text + "*";
    }

    currentCursor.insertText(newText);
}


void TextManipulationActionsDelegate::toggleSelectionStrikeout()
{
    QTextCursor currentCursor = mTextEdit->textCursor();
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;

    // Toggle bold
    QRegExp strikeoutRx("~{2}.*~{2}");

    if (text.indexOf(strikeoutRx) > -1)
    {
        newText = text.mid(2, text.size() - 4);
    }
    else
    {
        newText = "~~" + text + "~~";
    }

    currentCursor.insertText(newText);
}


void TextManipulationActionsDelegate::insertHorizontalLine()
{
    QTextCursor currentCursor = mTextEdit->textCursor();
    currentCursor.insertBlock();
    currentCursor.insertText("-------------------------------------------");
    currentCursor.insertBlock();
}


void TextManipulationActionsDelegate::insertModifyLink()
{
    QTextCursor currentCursor = mTextEdit->textCursor();
    QString selectedText = currentCursor.selectedText();

    InsertModifyLinkDialog dialog(mTextEdit);
    dialog.fromMarkdownFormat(selectedText);

    if (dialog.exec())
    {
        QString fmt = dialog.toMarkdownFormat();

        if (!fmt.isEmpty())
        {
            currentCursor.insertText(fmt);
        }
    }
}


void TextManipulationActionsDelegate::insertModifyImage()
{
    QTextCursor currentCursor = mTextEdit->textCursor();
    QString selectedText = currentCursor.selectedText();

    InsertModifyImageDialog dialog(mTextEdit);
    dialog.fromMarkdownFormat(selectedText);

    if (dialog.exec())
    {
        QString fmt = dialog.toMarkdownFormat();

        if (!fmt.isEmpty())
        {
            currentCursor.insertText(fmt);
        }
    }
}


void TextManipulationActionsDelegate::toggleSelectionUnorderedList()
{
    QTextCursor currentCursor = mTextEdit->textCursor();

    // note that paragraphs are separated by QChar::ParagraphSeparator,
    // or unicode U+2029 instead of line break characters.
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;
    QRegExp unorderedListkRx("^( )+\\* .*\u2029(( )+\\* .*\u2029)*");

    if (text.indexOf(unorderedListkRx) > -1) // match
    {
        newText = text.replace(QRegExp("( )+\\* "), "");
    }
    else
    {
        QStringList lines = text.split(QChar::ParagraphSeparator);

        foreach (QString line, lines)
        {
            QString tmp("  * %1\n");
            newText += tmp.arg(line);
        }

        newText.chop(1); // remove the last new line from the text
    }

    currentCursor.insertText(newText);
}


void TextManipulationActionsDelegate::toggleSelectionOrderedList()
{
    QTextCursor currentCursor = mTextEdit->textCursor();

    // note that paragraphs are separated by QChar::ParagraphSeparator,
    // or unicode U+2029 instead of line break characters.
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;
    QRegExp orderedListkRx("^( )+[0-9]+\\. .*\u2029(( )+[0-9]+\\. .*\u2029)*");

    if (text.indexOf(orderedListkRx) > -1) // match ordered list
    {
        newText = text.replace(QRegExp("( )+[0-9]+\\. "), "");
    }
    else // not a list
    {
        QStringList lines = text.split(QChar::ParagraphSeparator);

        for (int i = 0; i < lines.size(); i++)
        {
            QString tmp("  %1. %2\n");
            newText += tmp.arg(i + 1).arg(lines.at(i));
        }

        newText.chop(1); // remove the last new line from the text
    }

    currentCursor.insertText(newText);
}


void TextManipulationActionsDelegate::toggleSelectionBlockquote()
{
    QTextCursor currentCursor = mTextEdit->textCursor();

    // note that paragraphs are separated by QChar::ParagraphSeparator,
    // or unicode U+2029 instead of line break characters.
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;
    QRegExp blockquoteRx("^>(( )+.*)?\u2029(>(( )+.*)?\u2029)*");

    if (text.indexOf(blockquoteRx) > -1) // match
    {
        newText = text.replace(QRegExp(">(( )+)?"), "");
    }
    else
    {
        QStringList lines = text.split(QChar::ParagraphSeparator);

        foreach (QString line, lines)
        {
            QString tmp("> %1\n");
            newText += tmp.arg(line);
        }

        newText.chop(1); // remove the last new line from the text
    }

    currentCursor.insertText(newText);
}


void TextManipulationActionsDelegate::toggleHeadingHelper(int level)
{
    QTextCursor currentCursor = mTextEdit->textCursor();
    currentCursor.select(QTextCursor::LineUnderCursor);
    QString line = currentCursor.selectedText();

    QString newLine;
    int firstSpaceIndex = line.indexOf(" ");
    QString left = line.left(firstSpaceIndex);
    QString headingStart = QString("#").repeated(level);

    if (line.startsWith("#")) // line is a heading
    {
        line.replace(QRegExp("#+ "), "");

        if (left == headingStart) // case 1: same level we are expecting
        {
            newLine = line;
        }
        else // case 2: different levels
        {
            newLine = headingStart + " " + line;
        }
    }
    else // line is not a heading
    {
        newLine = headingStart + " " + line;
    }

    currentCursor.insertText(newLine);
}
