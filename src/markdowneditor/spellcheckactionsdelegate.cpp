#include "spellcheckactionsdelegate.h"
#include <QDebug>
#include <QScrollBar>
#include <QMenu>
#include <QRegularExpression>

const char* SpellCheckActionsDelegate::sTag = "[SpellCheckActionsDelegate]";

SpellCheckActionsDelegate::SpellCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                                     QSharedPointer<SpellCheck> spellCheck,
                                                     QObject* parent) :
    ProofreadActionsDelegateBase(textEdit, parent),
    mSpellCheck(spellCheck)
{
}


SpellCheckActionsDelegate::~SpellCheckActionsDelegate()
{
}


void SpellCheckActionsDelegate::runVisibleTextCheck(const QString& text, int startPos, int endPos)
{
    if (mSpellCheck.isNull())
    {
        return;
    }

    qDebug() << sTag << "Spell checking visible text";

    QTextCursor cursor = mTextEdit->cursorForPosition(QPoint(0, 0));
    mTextEdit->setTextCursor(cursor);
    mSelections.clear();

    QTextCharFormat fmt;
    fmt.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    fmt.setUnderlineColor(Qt::blue);

    while (mTextEdit->find(QRegularExpression("((?!_)\\w)+")))
    {
        QString word = mTextEdit->textCursor().selectedText();

        if (!mSpellCheck->spellcheck(word))
        {
            QTextEdit::ExtraSelection extra;
            extra.cursor = mTextEdit->textCursor();
            extra.format = fmt;
            mSelections.append(extra);
        }

        if (mTextEdit->textCursor().position() > endPos)
        {
            break;
        }
    }

    mTextEdit->setExtraSelections(mSelections);
}


void SpellCheckActionsDelegate::runDocumentCheck(const QString& text)
{
}


void SpellCheckActionsDelegate::showContextMenu(const QPoint& point)
{
    // Get word under cursor
    QTextCursor cursor = mTextEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText();

    // Are we on one of the possibly incorrect words?
    bool showSuggestions = false;

    foreach (QTextEdit::ExtraSelection selection, mSelections)
    {
        if (word == selection.cursor.selectedText())
        {
            showSuggestions = true;
            break;
        }
    }

    // Retrieve suggestions and show context menu with suggestions
    QStringList suggestions;

    if (showSuggestions)
    {
        suggestions = mSpellCheck->suggest(word);
    }

    showContextMenuWithSuggestions(point, suggestions);
}


void SpellCheckActionsDelegate::replaceSelectionWithTextReplacement()
{
    QObject* obj = sender();
    QAction* action = qobject_cast<QAction*>(obj);
    QString replacement = action->data().toString();

    // Replace selection
    QTextCursor cursor = mTextEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    cursor.insertText(replacement);

    checkVisibleText();
}

