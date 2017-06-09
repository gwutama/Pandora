#include "spellcheckactionsdelegate.h"
#include <QDebug>
#include <QScrollBar>
#include <QMenu>

const char* SpellCheckActionsDelegate::sTag = "[SpellCheckActionsDelegate]";

SpellCheckActionsDelegate::SpellCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                                     QSharedPointer<SpellCheck> spellCheck,
                                                     QObject* parent) :
    QObject(parent),
    mTextEdit(textEdit),
    mSpellCheck(spellCheck)
{
    connect(mTextEdit, &MarkdownTextEdit::customContextMenuRequested,
            this, &SpellCheckActionsDelegate::showContextMenu);
}


SpellCheckActionsDelegate::~SpellCheckActionsDelegate()
{
}


void SpellCheckActionsDelegate::spellcheckVisibleText()
{
    if (mSpellCheck.isNull())
    {
        return;
    }

    qDebug() << sTag << "Spell checking visible text";

    // Memorize positions
    // See: https://stackoverflow.com/questions/21955923/prevent-a-qtextedit-widget-from-scrolling-when-there-is-a-selection
    QTextCursor currentCursor = mTextEdit->textCursor();
    int scrollbarPosition = mTextEdit->verticalScrollBar()->value();

    // see: https://stackoverflow.com/questions/21493750/getting-only-the-visible-text-from-a-qtextedit-widget
    QTextCursor cursor = mTextEdit->cursorForPosition(QPoint(0, 0));
    QPoint bottomRight(mTextEdit->viewport()->width() - 1, mTextEdit->viewport()->height() - 1);
    int endPos = mTextEdit->cursorForPosition(bottomRight).position();
    mTextEdit->setTextCursor(cursor);

    // actual spell checking
    mSpellCheckSelections.clear();

    QTextCharFormat fmt;
    fmt.setUnderlineStyle(QTextCharFormat::DashDotDotLine);
    fmt.setUnderlineColor(Qt::red);

    while (mTextEdit->find(QRegExp("((?!_)\\w)+")))
    {
        QString word = mTextEdit->textCursor().selectedText();

        if (!mSpellCheck->spellcheck(word))
        {
            QTextEdit::ExtraSelection extra;
            extra.cursor = mTextEdit->textCursor();
            extra.format = fmt;
            mSpellCheckSelections.append(extra);
        }

        if (mTextEdit->textCursor().position() > endPos)
        {
            break;
        }
    }

    mTextEdit->setExtraSelections(mSpellCheckSelections);

    // Set positions back to previous ones
    mTextEdit->setTextCursor(currentCursor);
    mTextEdit->verticalScrollBar()->setValue(scrollbarPosition);
}


void SpellCheckActionsDelegate::showContextMenu(const QPoint& point)
{
    // Get word under cursor
    QTextCursor cursor = mTextEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText();

    // Are we on one of the possibly incorrect words?
    bool showSuggestions = false;

    foreach (QTextEdit::ExtraSelection selection, mSpellCheckSelections)
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


void SpellCheckActionsDelegate::showContextMenuWithSuggestions(const QPoint& point,
                                                               const QStringList& suggestions)
{
    mSuggestionActions.clear();

    QMenu* menu = mTextEdit->createStandardContextMenu();
    menu->addSeparator();

    for (int i = 0; i < suggestions.size(); i++)
    {
        QAction* action = menu->addAction(suggestions.at(i));
        action->setData(suggestions.at(i));

        connect(action, &QAction::triggered,
                this, &SpellCheckActionsDelegate::replaceSelectionWithTextReplacement);

        mSuggestionActions.append(action);
    }

    menu->exec(mTextEdit->mapToGlobal(point));
    delete menu;
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
}

